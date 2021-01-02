#pragma once
#if __has_include("ss.hpp")
	#include "ss.hpp"
#elif __has_include("2.hpp")
	#include "2.hpp"
#endif

#include <iostream>
#include <string>

namespace galaxy::solar_system::earth::europe::poland::drzewce
{
	int reader = 2;
}

//////////////////////

//tak wyglada funkcja , ktorej rezultat nie moze byc zignorowany
[[nodiscard]] int xf()
{
	return 5;
}

struct [[nodiscard]] ABC //wyniku tego typu nie mozna zignorowac
{
	///....
};

//[[maybe_unused]] moze byc umieszczone przed: nazwa zmiennej, argumentu lub funkcji

enum class [[nodiscard]] A {a,b,c};
//tutaj tak ,samo ; wyniku tego typu nie mozna zaniedbac

[[maybe_unused]]
void k([[maybe_unused]] int a)
{
	[[maybe_unused]] int k;
}

void func()
{
	int i {4};
	switch (i)
	{
		case 1:
			[[fallthrough]];
		case 2:
			//[[fallthrough]];
			//w ostatnim bloku dodanie fallthrough da blad kompilacji
			break;
	}
}

///////////////////////////////////////////////

enum class TYPE : char{A,B,C,D};

void fcn1()
{
	//od C++17 enumeratory moge inicjalizowac liczbami
	TYPE w{2};
	//TYPE e = {2};
	//nie zadziala
}


/////////////////////////////////////////////

//W C++17 bezposrednie inicjalizacje typow auto za pomoca {}
//zwracaja jakis typ zamiast std::initializer_list
//ale nie wiem ,czy to uzyteczne

class mamal
{
public:
	int heart = 0;
};

class horse : public mamal
{
public:
	std::string legs[4] = { "","","","" };
};

///////////////////////////////AGREGATY

//obiekty agregatow mozna od tak sobie inicjalizowac za pomoca klamr {}
//klasa jest agregatem ,gdy:
//-nie ma konstruktorow
//-nie ma niestatycznych danych private/protected
//-nie ma funkcji wirtualnych
//-moze dziedziczyc tylko public

struct agregateBase1
{
	int var1;
	double var2;
};

struct agregateBase2
{
	int var3;
	double var4;
};

struct agregate:
		public agregateBase1,
		public agregateBase2
{
	std::string str;
	bool pred;
};

void f2()
{
	agregate ag{{5,3.4},{4,4.4},"xd",true};
	//inicjalizacja agregatu, najpierw inicjalizujemy po kolei
	//klasy bazowe ,a na koncu klase koncowa
}

///WYKRYWANIE, CZY OBIEKT ZOSTAL ZNISZCZONY NATURALNIE Z POWODU KONCU ZASIEGU
/// CZY Z POWODU WYJATKU

class TestClass
{
	//...
	~TestClass()
	{
		//ponizsza funkcja zwraca 0 ,gdy obiekt normalnie zostal zniszczony
		//lub liczbe wyjatkow lecacych z jego powodu
		if (std::uncaught_exceptions() == 0)
		{
			//normalne sprzatanie..
		}
		else
		{
			//awaryjne sprzatanie..
		}
	}
};
