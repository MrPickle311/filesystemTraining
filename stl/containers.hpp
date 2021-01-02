#pragma once

#include <array>
#include <string>
#include <iostream>
#include <ostream>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <list>
#include <algorithm>
#include <queue>
#include <functional>

//Pamiętaj ,że lepiej jest używać metody .at(i) ,gdyż zgłaszają wyjątki!!!
//funkcje front() , back() nie wykonują sprawdzenia tego ,czy konetenr jest pusty
//vector<bool> → dynamiczne dodawanie i usuwanie bitów
//bitset → statyczne
//NIE USUWAJ NIC Z PUSTEGO KONTENERA
//UWAGA → jeśli coś zapewnia gwarancję silną lub podstawową np. niezgłaszania wyjątków i jeśli wykorzystuje np. mój operator porównania
//to on też nie może zgłaszać wyjątków


//wektory
//shrink_to_fit jest prośbą ,nie żądaniem

//deque
//dla wektorów szybkie jest wstawianie tylko elementów na końcu , jednak dla deque jest tak samo szybkie na obu końcach
//operacje iteratorowe są wolniejsze
//brak sterowania pojemnością oraz realokacją
//dodanie lub usunięcie jakiegoklowiek elementu powoduje unieważnienie wskaźnika ,referencji idt...
//szybszy proces realokacji
//zależnie od implementacji ,może zwalniać nieużywaną pamięć

//zwykłe listy
//wstawianie i usuwanie na każdej pozycji jest tak samo szybkie + nie unieważnia niczego
//zapewniają silną gwarancję dla niemal każdej operacji
//korzystaj z specjalnych funkcji zamiast algorytmów → są szybsze → przeadresowanie tylko wskaźników ,bez kopiowania
//operacje sort() oraz przypisanie mogą być niebezpieczne pod kątem wyjątków

//forward_list
//brak size()
//posiada specjalne funkcje np. insert_after, insert_before przyjmujące pozycję i wstawiające element odpowiednio za i przed nią
//wyjątki tam samo jak zwykłe listy

//zbiory i wielozbiory
//nie można bezpośrednio zmieniać wartości elementów → w celu zamiany najpierw usuwamy a potem wstawiamy element
//brak bezpośredniego dostępu do elementów
//kryterium sortowania można podać jako parametr szablonu ,lub przez konstruktor
//operacje modyfikacji zachowują wzajemne ułożenie elementów, takie same elementy są wstawiane na końcu równoważnych
//silna gwarancja dla jednoelementowych operacji wstawiania oraz wieloelementowych usuwania
//jak będe potrzebować kryterium sortowania ,to jest w książce

//mapy i multimapy
//para klucz-wartość musi definiować operacje przypisania oraz kopiowania
//klucz musi mieć operator porównywania
//wyszukiwanie elementów o określonej wartości jest słabe wydajnościowo
//nie można bezpośrednio zmieniać wartości kluczy → w celu zamiany najpierw usuwamy a potem wstawiamy element pod kluczem
//na stronie 377 na dole jest funkcja replace_key
//kryterium sortowania można podać jako parametr szablonu ,lub przez konstruktor
//wyszukiwanie odbywa się według klucza, upperboundy itp. działają tak samo
//w celu usuwania wg. wartości użyj algorytmu find,erase i lambdy
//operacje modyfikacji zachowują wzajemne ułożenie elementów, takie same elementy są wstawiane na końcu równoważnych
//silna gwarancja dla jednoelementowych operacji wstawiania oraz wieloelementowych usuwania, wszystko tak samo jak dla setów
//dodatkowo jeśli użyję operatora [] z kluczem ,dla którego węzeł nie istnieje , to zostanie utworzony nowy węzeł ,ale typ
//wartości musi mieć konstruktor domyślny!
//kryterium sortowania na stronie 388 na dole


//nieuporządkowane
//zamiast kryterium sortowania jest funkcja mieszająca
//klucz i wartość muszą umożliwiać operacje przypisania oraz kopiowania
//klucz musi mieć operator==
//1-parametr → typ m 2-gi → typ hashujący , 3-ci kryterium równoważności , 4-ty alokator
//wszystko jest umieszczone w tablicy mieszającej składającej się z kubełków , każdy kubełek to forward_list zawierająca wszystkie elementy
//dla których funkcja mieszająca zwróciłą tę samą wartość
//obsługują tylko operatory == i !=
//brak lower_bound itd...
//nie można korzystać z algorytmów wymagających iteratorów dwukierunkowych
//nie można bezpośrednio zmieniać wartości elementów → w celu zamiany najpierw usuwamy a potem wstawiamy element
//brak bezpośredniego dostępu do elementów
//Mogę : określić minimalną ilość kubełków, kryterium równoważności,funkcję mieszajacą,max poziom zapełnienia po którym następuje hashowanie
//minimalny współczynnik obciążenia → przemieszanie gdy maleje liczba elementów
//operacje powodujące przemieszanie : clear() ,reserve() ,insert() ,rehash()
//przemieszanie zachowuje względną kolejność
//własne kryterium równoważności 403 , funkcja mieszająca 401
//do usuwania elementów mogę używać tylko funkcji składowych
//operacja erase() zachowuje wzajemne ułożenie elementów, takie same elementy są wstawiane na końcu równoważnych
//przemieszanie unieważnia iteratory,ale referencje są ok
//operacje takie same jak na mapach i setach
//silna gwarancja dla jednoelementowych operacji wstawiania
//erase(),clear(),swap() nie zgłasza wyjątku → najlepsza gwarancja
//podstawowa gwarancja dla rehash()


//priority queue
//

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

void array_test()
{
	std::array<int,10> a;
	a.fill(5);
	int* b = a.data(); //zwracanie tablicy wskaźników
	for(size_t i{0}; i < 10 ;++i) std::cout << b[i] << " ";
	print("");
	std::array<int,10> h;
	h.fill(10);
	h.swap(a);
	print(h);
}

//prezentacja specjalnych funkcji dla list merge oraz splice
//merge jest nowym algorytmem w C++17 ,więc omówię w algorytmach
void list_test()
{
	print("List");
	//splice ogólnie przenosi elementy z jednego kontenera i umieszcza je w jakimś miejscu
	std::list<int> l1{1,2,3,4,5};
	std::list<int> l2{6,7,8,9,10};

	l1.splice(std::end(l1),l2); //wstawiam całą kolekcje
	print(l1);

	l2.push_back(11);

	l1.splice(std::end(l1),l2,std::begin(l2)); // biorę element z konkretnej pozycji
	print(l1);

	l2.push_back(12);
	l2.push_back(13);
	l2.push_back(14);

	l1.splice(std::end(l1),l2,std::begin(l2),std::end(l2)); //wstawiam zakres
	print(l1);
}

//specjalne operacje i funkcje
//wszystkie funkcje after i before działają na odpowiednio wcześniejszych i późniejszych elementach
void forward_list_test()
{
	print("Forward list");
	std::forward_list<int> l {1,2,3,4};
	auto itr {std::next(l.begin(),2)};
	l.insert_after(l.before_begin(),0); //wstawianie na początek
	print(l);

	l.push_front(-1);
	print(l);
	itr = l.begin();
	std::advance(itr,2);
	l.insert_after(itr,9);
	print(l);
}

//wersja szybka rówież można przerobić na iteratory oraz zasięgi
//ale zaburza względny porządek elementów
template<typename T>
void quick_remove(std::vector<T>& v,size_t idx)
{
	if(idx < v.size())
	{
		std::swap(v[idx],v.back());
		v.pop_back();
	}
}

//prezentacja metody skutecznego usuwania elementów
void vector_test()
{
	print("Vectors");
	std::vector<int> v{1,2,3,4,2,2,55,2,3,2,4,5,3};
	v.emplace_back(6);
	quick_remove(v,0);
	print(v);

	auto new_end {std::remove(v.begin(),v.end(),2)};
	v.erase(new_end,v.end());
	print(v);
}

void sets_test()
{
	print("Sets");
	std::set<int> s1{2,3,4,2,1,3,2,2,4,6};
	if(s1.insert(2).second)//funkcja insert zwraca parę (iterator,bool) → (miejsce wstawienia,czy operacja się powiodła)
		print("2 done!");
	print(s1);
	print(std::string{"upper bound 5: " }.append(std::to_string(*s1.upper_bound(5))));//zwraca ostatnią pozycję na której zostałby wstawiony element o
	//wartości 5
	print(std::string{"lower bound 5: " }.append(std::to_string(*s1.lower_bound(5))));//zwraca 1-szą pozycję na której zostałby wstawiony element o
	//wartości 5
	print(std::string{"equal range 5: " }.append(std::to_string(*s1.upper_bound(5))));//zwraca 1-szą i ostanią pozycję na której zostałby wstawiony
	//element owartości 5
	print(*s1.find(5));
	print(s1.count(5));
	if (s1.find(5) == s1.end()) print("Not found!");

	print(s1.erase(6)); // dla setów erase zwraca 1 lub 0

	std::multiset<int> s2{2,2,2,3,3,3,4,42,2,42,31};
	print(s2.erase(2)); // usuwa wszystkie
	//usuwanie tylko jednego
	auto ptr = s2.find(3);
	if(ptr != s2.end())
		print(*s2.erase(ptr));

}

void map_test()
{
	print("Map test");
	std::map<int,int> m1{{1,1},{2,1},{3,1},{4,2}};

	for(const auto& x :m1)
		print(x.second);

	m1[4] = 7;//jednak można modyfikować
	print(m1[4]);

	//wstawianie krotkowe   //argumenty konstruktora typu klucza //argumenty konstrukora typu wartości
	m1.emplace(std::piecewise_construct,std::make_tuple(89),std::make_tuple(6));

	//usuwanie elementów o odpowiedniej wartości
	//UWAGA
//	for(auto pos {m1.begin()}; pos != m1.end() ; ++pos)
//	{
//		if(pos->second == 1)
//			m1.erase(pos);
//	}
	//taki kod jest niebezpieczny ,gdyż mogę uszkodzić sobie iterator i pętlę chuj strzeli

	//należy to robić tak
	for(auto pos {m1.begin()}; pos != m1.end();)
	{
		if(pos->second == 1)
			pos = m1.erase(pos); //od C++11 ta metoda zwraca następną pozycję
		else ++pos;
	}

	m1[5] = 100; //wolniejszy sposób wstawiania ,lepiej emplace,insert
	m1[6]; //odpalenie konstruktora domyślnego
	print(m1[5]);
	print(m1[6]);
	print(m1[7]);//również odpalenie konstruktora domyślnego

	//jeżeli chcę coś odczytać i jednocześnie nie wstawiać nowego elementu to należy użyć .at()
	try
	{
		print(m1.at(8));
	}
	catch (std::out_of_range& e)
	{
		print("Cannot show this : ");
	}

}

void unordered_tests()
{
	print("Unordered containers");
	std::unordered_set<int> s1(5); //liczba kubełków
	print(s1.max_load_factor());
	print(s1.bucket_count());
	s1.max_load_factor(0.7);//jeśli idę w stronę 0 ,to chcę zmniejszyć zużycie pamięci ,jeśli do 1 → szybkość
							//dobry kompromis to 0.7, domyślnie jest 1 dla superszybkości
	s1.rehash(100); // przygotuj się na 100/max_load_factor() ( 100/0.7 ) elementów
	print(s1.bucket_count());
	s1.reserve(100); //przygotuj się na 100 elementów → 143 kubełki
	print(s1.bucket_count());
	s1.insert(3);
	s1.insert(3);
	s1.insert(35);
	s1.insert(36);
	s1.insert(5);
	s1.insert(7);
	print(s1.count(3));
	print(*s1.find(7)); //zawsze korzystaj z funkcji składowych do poszukiwania są najszybsze (tylko 1 porównanie) ze wszystkich kontenerów
	print(s1.bucket_size(7)); // liczba elementów w danym kubełku
	print(*s1.begin(7));//zwraca iterator jednokierunkowy dla listy z danego kubełka
}

void priority_queue_test()
{
	//priorytetowość oznacza się po prostu jako kryterium sortowania
	std::priority_queue<int,std::deque<int>,std::greater<int>> q;
	q.push(1);
	q.push(5);
	q.push(0);
	q.push(-12);
	q.push(-11);
	q.push(1213);
	q.push(14);
	q.push(123);
	q.push(13);
	print(q.top());
}

void container_main()
{
	array_test();
	list_test();
	forward_list_test();
	vector_test();
	sets_test();
	map_test();
	unordered_tests();
	priority_queue_test();
}
