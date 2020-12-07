#pragma once

//1.dobry do ogarniecia przestarzalego kodu
//2.adapter mozna poznac po konstruktorze przyjmujacym
//instancje innego typu abstrakcji/interfejsu
//3.Wzorzec ten stanowi warstwe posredniczaca pomiedzy moim kodem
//,a otoczeniem

///Zastosowanie:
///1.Stosowac klaase adaptera ,gdy chce wykorzystac jakas istniejaca
/// klase ,ale jej interfejs nie jest kompatybilny z reszta kodu
///2.Gdy chce wykorzystac ponownie wiele istniejacych podklas ktorym brakuje
/// jakiejs wspolnej funkcjonalnosci , niedajacej sie dodac do ich nadklasy
/// ,wted adapter dziedziczy nadklase i przyjmuje podklasy
/// lub opakowac je w postaci szablonu, bardzo podobne do Dekoratora

//Jak zaimplementowac:
//1.Upewnic sie ,ze mam przynajmniej dwie klasy o niekompatybilnych interfejsach
//jakas klasa uslugowa niepasujca do kodu klienckiego
//2.Zadeklarowac interfejs klienta i opisac jak ma wygladac komunikacja klientow
//z usluga
//3.Stworzyc klase adaptera zgodna z interfejsem klienckim
//4.Stworzyc adapter poprzez kompozycje lub dziedziczenie
//5.Implementacja wszystkich metod interfejsu klienta w klasie adaptera.
//Adapter powinien delegowac wiekszosc faktycznej pracy obiektowi oferujacemu
//usluge i zajmowac sie wylacznie posrednictwem lub konwersja danych
//6.Klienci powinni stosowac adapter za posrednictwem interfejsu klienta.
//Brak wplywu na kod kliencki

///Zalety:
///1.Spelniona Single Responsible Priniciple
///2.Spelniona  Open/closed Principle <-- brak wplywu na kod klienta,
/// tylko rozszerzenia

//Wada:
//Zwieksza sie zlozonosc kodu

///Powiazania z innymi wzorcami:
///1. Most wykorzystuje sie od poczatku projektu ,by pozwolic na niezalezna
/// prace nad poszczegolnymi czesciami aplikacji, lecz Adapter jest rozwiazaniem
/// stosowanym w istniejacej aplikacji w celu umozliwienia wspolpracy pomiedzy
/// niekompatybilnymi klasami
///2.Adapter zmienia interfejs istniejacego obiektu zas Dekorator rozszerza go
/// bez zmiany interfejsu. Dekorator wspiera rekursywna kompozycje,co nie jest
/// mozliwe dla adaptera
///3.Adapter wyposaza opakowany obiekt w inny interfejs. Pelnomocnik w taki sam
/// ,zas Dekorator wprowadza rozszerzony interfejs
///5. Fasada definiuje nowy interfejs istniejacym obiektom ,zas Adapter zaklada
/// zwiekszenie uzytecznosci zostanego interfejsu. Adapter na ogol opakowuje
/// pojedynbczy obiekt ,zas Fasada obejmuje caly podsystem obiektow
/// 6.Most,Stan,Strategia i pewnym stopniu Adapter maja podobna strukture.
/// Wszystkie oparte sa na kompozycji ,co oznacza delegowanie zadan innym
/// obiektom . Jednak kazdy z tych wzorcow rozwiazuje inne problemy. Wzorzec
/// nie jest bowiem tylko recepta na ustrukturyzwanie kodu w pewien sposob,lecz
/// takze informacja dla innych deweloperow o charakterze rozwiazywanego problemu

#include <string>
#include <algorithm>
#include <iostream>

//Target definiuje specyficzne dla domeny interfejs uzywany przez kod
//kliencki

class Target
{
public:
	virtual ~Target() {}

	virtual std::string request() const
	{
		return "Target: The default  target's behaviour";
	}
};

//Adaptee zawiera przydatne zachowanie, ale interfejs jest niekompatybilny
//z obecnym kodem klienckim. Adaptee potrzebuje byc zaadaptowany
//zanim zostanie uzyty w kodzie klienta

class Adaptee
{
public:
	std::string specificReqest() const
	{
		return ".eetpadA eht fo roivaheb laicepS";
	}
};

//Adapter stwarza kompatybilny interfejs dla Adaptee

class Adapter:
		public Target
{
private:
	Adaptee* adatpee_;

public:
	Adapter(Adaptee* adatpee):adatpee_{adatpee}
	{}
	// Target interface
public:
	virtual std::string request() const override
	{
		std::string to_reverse  {this->adatpee_->specificReqest()};
		std::reverse(to_reverse.begin(),to_reverse.end());
		return "Adapter: (TRANSLATED) " + to_reverse;
	}
};

void clientCode(const Target* target)
{
	std::cout << target->request();
}

void a_main()
{
	std::cout << "Client: I can work just fine with the Target objects:\n";
	Target* target {new Target{}};
	clientCode(target);
	std::cout << "\n\n";
	Adaptee* adaptee {new Adaptee{}};
	std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
	std::cout << "Adaptee " << adaptee->specificReqest();
	std::cout << "\n\n";
	std::cout << "Client: But I can work with it via the Adapter:\n";
	Adapter* adapter {new Adapter{adaptee}};
	clientCode(adapter);
	std::cout << "\n\n";

	delete target;
	delete adaptee;
	delete adapter;
}
