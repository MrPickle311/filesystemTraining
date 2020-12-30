#pragma once

#include <iterator>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <type_traits>

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
}
