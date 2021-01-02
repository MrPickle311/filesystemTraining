#pragma once

#include <any>
#include <optional>
#include <variant>
#include <iostream>
#include <string>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <list>
#include <tuple>

std::optional<int> read_int()
{
	int i;
	std::cout << "input:\n";
	if (std::cin >> i)
		return i;
	return {};//niepowodzenie lub brak wartosci
}

std::optional<int> operator+ (std::optional<int> a,std::optional<int> b)
{
	if (!a || !b)//konwersja std::optional<> na bool
		return {};//jesli ,a lub b nie zawiera wartosci ,to zostanie zwrocony brak wartosci
	return {*a + *b};//wyluskanie wartosci z std::optional<int> za pomoca operatora *
}

std::optional<int> operator+ (std::optional<int> a,int b)
{
	if (!a)//konwersja std::optional<> na bool
		return {};//jesli ,a lub b nie zawiera wartosci ,to zostanie zwrocony brak wartosci
	return {*a + b};//wyluskanie wartosci z std::optional<int> za pomoca operatora *
}

struct A1
{
	int a_;
	std::string b_;
};

std::optional<A1> read_struct()
{
	if(int a ; std::cin >> a)
		if (std::string b; std::cin >> b)
			return A1{a,b};
	return {};
}

void optional_tests()
{
	std::cout << "OPTIONAL TESTS" << std::endl;
	auto a {read_int()};
	auto b {read_int()};
	auto sum{a + b + 10};
	//uzycie tych operatorow
	if(sum)//jesli sum nie jest puste
		std::cout << "SUM = " << *sum << std::endl;
	else std::cout << "SUM jest puste! " << std::endl;

	std::optional<A1> a1{read_struct()};
	if(a1)
	{
		std::cout << "A1 jest nie puste i jego wartosci to:\n!" << std::endl;
		std::cout << a1->a_ << "\n" << a1->b_;
	}
	else std::cout << "A1  jest puste!" << std::endl;
}

void print(const std::any& a)
{
	if(!a.has_value())
		return;
	else if (a.type() == typeid (std::string))
		std::cout << "Przeslano stringa : " << std::quoted(std::any_cast<const std::string&>((a))) << '\n';
										//									 ------------^^------
										//uzywam ampersanda w celu unikniecia kopiowania przy rzutowaniu
	else if (a.type() == typeid (int))
		std::cout << "Przeslano inta : " << std::any_cast<int>(a) << '\n';
	else if (a.type() == typeid (std::list<int>))
	{
		const auto& l {std::any_cast<const std::list<int>& >(a)};
		//									------------^^------
		//uzywam ampersanda w celu unikniecia kopiowania przy rzutowaniu
		std::cout << "Przyslano liste intow :\n";
		std::copy(l.begin(),l.end(),std::ostream_iterator<int>{std::cout," "});
		std::cout << "\n";
	}
	else std::cout << "Nie rozpoznano typu danych !" << std::endl;
}

void any_test()
{
	print({});
	print("abc");
	print(std::string{"abc"});
	print(123);
	print(std::list<int>{1,2,3,4});
	print(std::any{std::in_place_type_t<std::list<int>>{}, {1,2,3}});
	// w celu unikniecia operacji kopiowania duzych obiektow robie tak
	//jako pierwszy argument konstruktora przekazuje std::in_place_type_t<T>
	//ktory zawiera full info o tworzonym typie ,a drugi argument to
	//std::initializer_list ,ktory zapewnia argumenty
}

class cat
{
	std::string name_;
public:
	cat(std::string n):name_{n} {}
	void meow() const
	{
		std::cout << name_ << " meow!" << std::endl;
	}
};

class dog
{
	std::string name_;
public:
	dog(std::string n):name_{n} {}
	void woof() const
	{
		std::cout << name_ << " woof!" << std::endl;
	}
};

template<typename T>
bool is_type(std::variant<dog,cat>& v)
{
	return  std::holds_alternative<T>(v);//ta funkcja sprawdza ,czy obecny std::variant
	//przechowuje aktalnie typ T
}

struct animal_voice
{
	void operator() (const dog& d) const {d.woof();}
	void operator() (const cat& c) const {c.meow();}
	void operator() (const std::monostate&) {std::cout << "Brak wartosci!" << std::endl;}
};

void variant_test()
{
	std::list< std::variant<dog,cat,std::monostate> > list {cat{"Tuba"},  dog{"Balou"}, cat{"Bobby"}, std::monostate{}};
	//							----^^^^^^^^^^^^^^^----
	//					std::monostate oznacza ,ze std::variant jest zainicjalizowany bez wartosci

	//metoda bardziej nastawiona na metaprogramowanie
	for (const auto& animal : list)
		switch (animal.index())
		{
			case 0:
				std::get<dog>(animal).woof();
				break;
			case 1:
				std::get<cat>(animal).meow();
				break;
		}
	std::cout << "-----\n" << std::endl;

	//metoda na wskaznikach
	for (const auto& animal : list)
		if (const auto d {std::get_if<dog>(&animal)}; d)//jesli w animal jest obecny typ dog ,to funkcja get_if
			d->woof();									//zwraca wskaznik typu dog do obiektu d
		else if (const auto c {std::get_if<cat>(&animal)}; c)
			c->meow();
	std::cout << "-----\n" << std::endl;

	//elegancka i prosta metoda
	for (const auto& animal : list)
		std::visit(animal_voice{},animal);//funkcja ta akceptuje obiekt funkcji oraz egzemplarz std::variant
	//obiekt funkcji musi implementowac przeciazone operatory () wszystkich typow ,ktore variant przechowuje
	//i to jest najfaniejsza metoda

}

void tuple_test()
{
	std::tuple<int,int,std::string,double> t1;

	t1 = std::make_tuple(1,2,"abc",1.0);
	auto t2 {std::make_tuple(1,2,"abc",1.0)};

	int x {std::get<0>(t1)};
	//indeksowanie po parametrach szablonu odbywa sie w TYLKO czasie kompilacji

	std::string str {"meil"};

	auto y {std::make_tuple(std::ref(str))};// otrzymam std::tuple<std::string &>
	//takie podejscie pozwala na wyodrebnianie wartosci z krotki

	int i;
	float f;
	std::string s;

	std::tuple t3{std::ref(i),std::ref(f),std::ref(s)};//dostep do elementow krotki za pomoca niej samej
	//oraz zmiennych i,f,s

	//lub mozna zrobic to tak
	std::tuple<int,float,std::string> t4{1,2.0,"se"};
	int i1;
	float f1;
	std::string s1;
	std::tie(i1,f1,s1) = t4;


	//lub ,gdy czesciowo chce sobie wydzielic elementy krotki
	std::tuple<int,float,std::string> t5{1,2.0,"se"};
	int i2;
	std::string s2;
	std::tie(i2,std::ignore,s1) = t4;

	//nie mozna konwertowac std::initializer_list na krotke

	size_t size {std::tuple_size<decltype(t5)>::value};//zwraca ilosc elementow w krotce

	std::tuple_element<2,decltype(t5)>::type s3; //tuple_element zwraca typ elementu krotki

	auto q {std::tuple_cat(t4,t5)}; //tworzy 6-cio elementowa krotke z 2 innych krotek
}

//std::forward oraz różnica między nim a std::move

//std::forward wraz z forwarding reference przeprowadza parametry w niezmienionej postaci tzn. lvalue pozostawia
//lvalue , a rvalue pozostawia rvalue
//,stosuje się go do
//forwarding reference
//std::forward stosuje się głównie przy szablonach ,gdy nie wiadomo ,czy argument
//jakiejś funkcji szablonowej(również konstruktora) będzie rvalue ,czy lvalue
//w skrócie :: std::forward idealnie przekazuje argumenty, tylko z rvalue robi rvalue

//std::move przenosi obiekty

void fr(const int& arg)
{
	std::cout<< "by lvalue!\n";
}

void fr(int&& arg)
{
	std::cout << "by rvalue!\n";
}

template<typename T>
void frw(T&& arg)// T&& to jest tz. forwarding reference
{
	std::cout << "via std::forward\n";
	fr(std::forward<T>(arg));
	std::cout << "via std::move\n";
	fr(std::move(arg));
	std::cout << "by simple passing\n";
	fr(arg);
}

void forward_test()
{
	std::cout << "sending rvalue = 3\n";
	frw(5);//tutaj arg będzie rvalue
	std::cout << "sending lvalue = 3\n";
	int a {5};//tutaj arg będzie lvalue
	frw(a);
}

void util_main()
{
	//optional_tests();
	//any_test();
	//variant_test();
	//tuple_test();
	forward_test();
}
