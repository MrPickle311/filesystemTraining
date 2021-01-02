#pragma once

#include <iterator>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <type_traits>
#include <sstream>
#include <unordered_set>

//Tryb dokładnego debugowania STl:
//do polecenia kompilatora dodać opcję -D_GLIBCXX_DEBUG

/*hierarchia iteratorów:
 * 1.wejściowe- jednorazowy odczyt , iteracja = odczyt i przejście -> istream
 * 2.forward - możliwość odczytu wiele razy z tego samego miejsca -> forward_list , nieuporządkowane kontenery
 * 3.bidirectional - w dwie strony -> listy, kontenery asocjacyjne
 * 4.random access - swobodny -> wektory,stringi,array,cstringi,deque
 */

template<typename T,typename Arg>
std::ostream& operator<< (const T&,const Arg&);

template<typename T, typename Arg = T>
struct EqualExists
{
   enum { value = !std::is_same<decltype(std::declval<T>() < std::declval<Arg>()), std::ostream&>::value };
};

template<typename  T,typename N = typename T::value_type>
void print_impl(const T& container, std::false_type)
{
	std::copy(container.begin(),container.end(),
				  std::ostream_iterator<N>{std::cout," "});
	std::cout << '\n';
}

template<typename T>
void print_impl(const T& scalar, std::true_type)
{
	std::cout << scalar << '\n';
}

template<typename T>
constexpr const bool check_string_valid()
{
	return std::is_convertible<T,std::string>::value ||
		   std::is_same<T,std::string>::value;
}

template<typename T>
void print(const T& obj)
{
	if constexpr (check_string_valid<T>())
		print_impl<std::string>(obj,std::true_type{});
	else
		print_impl<T>(obj,std::is_scalar<T>{});
}

void helper_functions()
{
	//advance
	/*dla bi-iteratorów przesunięci n może być ujemne,lecz całkowite
	 * ta funkcja nie sprawdza zakresu
	 */
	std::vector<int> v{1,2,3,4,5,6,7};
	decltype (v)::iterator i{v.end()};
	std::advance(i,-4);

	//next,prev
	//nie sprawdzają zakresu
	//next = distance(itr,n) , prev = distance(itr,-n)
	decltype (v)::iterator  i1 = std::prev(i,3);
	decltype (v)::iterator  i2 = std::next(i,2);
	print(*i1);
	print(*i2);

	//distance(i1,i2)
	//Pamiętaj ,że i2 musi być dalej niż i1 dla iteratorów nieswobodnych
	//jednak słaba wydajność dla iteratorów innych niż swobodnego dostępu
}

void inverse_iterators()
{
	print("Inverse iterators");
	using itr = std::list<int>::iterator;
	using ritr = std::list<int>::reverse_iterator;
	std::list<int> l{23,23,545,5623,423,423,321};
	std::for_each(l.rbegin(),l.rend(),[=](int& i)
	{
		print(i);
		i = 10;
	});
	print(l);
	//konwersje między iteratorami
	itr i1 {l.begin()};
	*i1 = 2;
	++i1;
	*i1 = 2;
	print(l);
	ritr i2{i1};
	--i2; //tutaj muszę się cofnąć w prawo, gdyż konwersja na iterator odwrotny pcha mnie o 1 w lewo
	*i2 = 4;//UWAGA -> przy konwersji sprawdzaj poprawność zakresu ,std::list<int>::reverse_iterator{l.begin()} wskaże na miejsce przed 1-szym elementem
	++i2;
	*i2 = 4;
	print(l);
	//konwersja spowrotem,
	itr i3 {i2.base()};
	--i3;// tutaj muszę się cofnąć o jeden w lewo
	*i3 = 6;
	++i3;
	*i3 = 6;
	print(l);

}

void inserters() //wstawiacze -> iteratory wyjściowe -> tylko zapis
{//patrz ,które algorytmy wymagają jakich iteratorów
	print("Inserters");
	using itr = std::list<int>::iterator;
	std::list<int> l{1,2,3};

	print(l);
	//rozkazy ++itr,itr++ , --itr nic nie robią ,są puste
	std::back_insert_iterator<std::list<int>> it{l};
	it = 10;
	it = 3;
	it = 7;//same się przesuwają
	print(l);
	//można jeszcze tak
	std::back_inserter(l) = 50; // to jest funkcja ,lol
	std::back_inserter(l) = 70;
	print(l);
	//analogicznie front_inserter
	/*Uwaga! Zanim użyjesz algorytmu na kontenerze ,który się realokuje (deque,vetor) zaalokuj odpowiednią ilość miejsca
	 * by iteratory nie straciły ważności !!!
	 */
	std::vector<int> dest{1,2,3};
	std::vector<int> src{4,5,6};
	dest.reserve(dest.size() * 2); // przed takimi operacjami zwiększaj rozmiar!!!!
	std::copy(src.begin(),src.end(),std::back_inserter(dest));
	print(dest);

	//wstawiacz ogólny , inicjalizowany jest pozycją początkową oraz kontenerem
	std::insert_iterator<std::list<int>> insrt{l,l.begin()};
	*insrt = 40;
	++insrt;
	*insrt = 50;
	++insrt;
	print(l);
	std::inserter(l,std::next(l.begin(),2)) = 60;
	std::inserter(l,std::next(l.begin(),3)) = 70;
	print(l);
}

void stream_iterator()
{
	//różnią się od wstawiaczy ,tym ,że zamiast operatora= wykorzystują operator<< lub operator>>
	print("Stream iterators");
	using itr = std::list<int>::iterator;
	std::list<int> l{1,2,3};

	std::string str{"ABCDEF"};
	std::ostringstream s{str};

	std::ostream_iterator<char> sq{s," "};
	*sq = 'G';
	++sq;
	*sq = 'I';
	++sq;
	std::cout << s.str() << '\n';
	print(str);

	std::ostream_iterator<int> outitr{std::cout," , "};
	for(auto x : l) outitr = x;
	print("");
	std::ostream_iterator<char> strout{std::cout," "};
	for(auto x : str) strout =x;
	print("");
	std::copy(l.begin(),l.end(),std::ostream_iterator<int>{std::cout," "});
	print("");

	//Iteratory wejściowe
	print("Iteratory wejściowe");

	//dwa iteratory wejściowe są sobie równe jeśli
	//1.są iteratorami końca strumienia → nie mogą prowadzić odczytu
	//2.obydwa mogą prowadzić odczyt i wykorzystują ten sam strumień

	l.clear();
	std::istream_iterator<int> reader{std::cin};
	std::istream_iterator<int> eof;//pusty iterator to eof

	std::ostream os{std::cout.rdbuf()};
	while (reader != eof)
	{
		os << *reader;
		++reader;
	}

	print("");

	std::istream_iterator<std::string> reader1{std::cin};
	std::istream_iterator<std::string> eofstr;
	std::ostream_iterator<std::string> strout2{std::cout," "};
	while (reader1 != eofstr)
	{
		std::advance(reader1,2);
		if(reader1 != eofstr)//wywołanie operatora* dla końca strumienia powoduje niezdefiniowane zachowanie
			*strout2++ = *reader1++;
	}
}

void move_iterators()
{
	//pamiętaj ,że jeśli stosujesz iteratory przenoszące,to możesz uzyskać dostęp do elementów tylko raz
	//gdyż zostaną przeniesione → tylko iteratory wejściowe będą zawsze bezpieczne
	std::list<int> l1{1,2,3,4,5};
	//std::move_iterator<std::list<int>> start{l1.begin()};
	//std::move_iterator<std::list<int>> end{l1.end()};
	//std::list<int> l2{start,l1.end()};
}

//Definicja własnych funkcji dla iteratorów standardowych

//1.Stworzenie funkcji bazowej przekierowującej w zależności od typu iteratora
//2.Stworzenie konkretnych implementacji

template<typename BiIterator>
void foo(BiIterator beg,BiIterator end,std::bidirectional_iterator_tag)
{
	//...operacje
}

template<typename RaIterator>
void foo(RaIterator beg,RaIterator end,std::random_access_iterator_tag)
{
	//...operacje
}

//funkcja-mediator
template<typename Iterator>
inline void foo(Iterator beg,Iterator end)
{
	foo (beg,end,std::iterator_traits<Iterator>::iterator_category());
}


//Definicja własnego iteratora

//Są dwa sposoby
//1.Podać pięć niezbędnych definicji typów dla ogólnej struktury iterator_traits
	/* 1.kategoria iteratora
	 * 2.typ elementu
	 * 3.typ różnicy -opcjonalne
	 * 4.typ wskaźnikowy -opcjonalne
	 * 5.typ referencji -opcjonalne
	 */
//2.Zdefiniować częściową specjalizację struktury iterator_traits


//sposób 1,można użyć do asocjacyjnych i nieuporządkowanych
template<typename Container>
class asso_insert_iterator:
		public std::iterator < std::output_iterator_tag, typename Container::value_type>
{
protected:
	Container& container_;
public:
	explicit asso_insert_iterator(Container& c):
		container_{c}
	{}

	//wstawienie danych do kontenera
	asso_insert_iterator<Container>& operator= (const typename Container::value_type& value)
	{
		container_.insert(value);
		return *this;
	}
	//operacja dereferencji jest pusta, gdyż zwraca sam iterator
	asso_insert_iterator<Container>& operator* ()
	{
		return *this;
	}
	//operacja inkrementacji jest pusta, gdyż zwraca sam iterator
	asso_insert_iterator<Container>& operator++ ()
	{
		return *this;
	}
	asso_insert_iterator<Container>& operator* (int)
	{
		return *this;
	}
};

//wykorzystanie
void asso_test()
{
	std::unordered_set<int> set;

	asso_insert_iterator<std::unordered_set<int>> itr{set};
	*itr = 1;
	*itr = 2;
	*itr = 3;
	print(set);

	std::vector<int> v{1,2,3,4,5};
}

void iterator_main()
{
	std::vector<int> v{1,2,3,4,5,6};
	std::vector<int>::iterator i1 {v.begin()};
	std::vector<int>::iterator i2 {v.end()};
	std::swap(i1,i2);
	std::for_each(i2,i1,[=](int& i)
	{
		std::cout << i << '\n';
	});

	//Insertery

	std::back_insert_iterator<std::vector<int>> ins{v};

	*ins = 6;
	++ins;
	*ins = 10;
	++ins;
	*ins = 20;
	++ins;
	//std::cout << v.back();
	print(v);
	i1 = v.begin();
	i2 = v.end();
	--i2;
	std::iter_swap(i1,i2); //ta funkcja po prostu zamienia wartości , które kryją się pod pozycjami
	print(v);
	for(size_t i{0}; i < v.size()/2 - 1;++i) //imitacja reverse
	{
		std::iter_swap(i1,i2);
		++i1;
		--i2;
	}
	print(v);

	//jednak iteraotory wektora są dostępu swobodnego
	v.begin()[2] = 30; //nieważne na jakiej jestem pozycji ,to i tak mogę mieć dostęp do każdej innej
	print(v);
	print(std::string{"sdasdas"});
	helper_functions();
	inverse_iterators();
	inserters();
	stream_iterator();
	asso_test();
}
