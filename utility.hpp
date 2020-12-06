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

void util_main()
{
	//optional_tests();
	//any_test();
	variant_test();
}
