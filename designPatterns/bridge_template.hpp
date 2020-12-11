#pragma once

//Podzial logiki biznesowej lub duzej klasy w taki sposob by powstale
//osobne hierarchie klas pracowaly niezaleznie od siebie

//Jedna z hierarchii to Abstrakcja posiadajaca referencje do obiektu
//drugiej hierarchii zwanej implementacja

//Szczegolnie przydatne , gdy trzeba wspierac obsluge wielu typow
//serwerow bazodanowych lub interfejsow programowania aplikacji
//danego typu

//Mozna rozpoznac po wyraznym rozdzieleniu na czesc kontrolujaca i wiele
//roznych platform od ktorych ta czesc zalezy

///Zastosowanie:
/// 1.Gdy chce rozdzielic i przeorganizowac monolityczna klase posiadajaca
/// wiele wariantow takiej samej funkcjonalnosci. Rozbicie klasy na kilka
/// hierarchii klas
/// 2.Gdy chce rozszerzyc klase na kilku niezaleznych plaszczyznach.
/// Plaszczyzny mozna reprezentowac jako hierarchie klas. Pierwotna
/// klasa deleguje prace do poszczegolnych hierarchii
/// 3.Gdy chce miec mozliwosc wyboru implementacji w czasie dzialania
/// programu

///Jak zaimplementowac:
/// 1.Okreslic plaszczyzny/hierarchie klas, abstrakcja/platforma ,
/// front-end/back-end , interfejs/implementacja
/// 2.Okresl jakie operacje sa klientowi potrzebne i zdefiniuj je w
/// bazowej klasie abstrakcji
/// 3.Okreslic zakres operacji dostepnych na wszystkich platformach.
/// Zadeklarowac te ,ktorych abstrakcja potrzebuje w ogolnym interfejsie
/// implementacji
/// 4.Stworzenie konkretnych klas implementacji
/// 5.Dodanie pola do interfejsu abstrakcyjnego
/// 6.Ewentualne rozszerzenie abstrakcji
/// 7.Kod kliencki powinien przekazywac obiekt implementacji konstruktorowi
/// abstrakcji i tyle , nic wiecej nie powinien moc zrobic z implementacja


///Zalety:
/// 1.Mozna tworzyc niezalezne od platformy klasy i aplikacje
/// 2.Klient nie musi sie przejmowac szczegolami platformy
/// 3. Spelnia Open/Closed Principle
/// 4. Spelnia Single Responsibility Principle

///Wada:
/// Kod moze sie skomplikowac gdy zastosuje sie ten wzorzec dla wysoce
/// zwartej klasy

///Powiazania z innymi wzorcami:
/// 1.Most zazwyczaj wykorzystuje sie na poczatku projektu ,by pozwolic na
/// niezalezna prace nad poszczegolnymi czesciami aplikacji. Z drugiej
/// strony Adapter jest rozwiazaniem dla istniejacej aplikacji w celu
/// umozliwienia wspolpracy miedzy niekompatybilnymi klasami
/// 2.Most,Stan,Strategia i pewnym stopniu Adapter maja podobna strukture.
/// Wszystkie oparte sa na kompozycji ,co oznacza delegowanie zadan innym
/// obiektom . Jednak kazdy z tych wzorcow rozwiazuje inne problemy. Wzorzec
/// nie jest bowiem tylko recepta na ustrukturyzwanie kodu w pewien sposob,lecz
/// takze informacja dla innych deweloperow o charakterze rozwiazywanego problemu
/// 3.Fabryka abstrakcyjna moze byc stosowana z Mostem. Takie sparowanie jest
/// uzyteczne ,gdy niektore abstrakcje zdefiniowane przez Most moga
/// wspoldzialac wylacznie z okreslonymi implementacjami .W tym przypdku
/// Fabryka abstrakcyjna moze hermetyzowac te relacje i ukryc zawilosci
/// przed kodem klienckim
///4.Budowniczy i Most: klasa kierownik pelni role abstrakcji ,zas
/// poszczegolni budowniczy stanowia implementacje

#include <string>
#include <iostream>

///Implementation definiuje interfejs dla wszystkich klas implementacji.
///Nie musi pasowac do interfejsu abstrakcyjnego. W sumie te dwa interfejsy
///moga byc calkowicie odmienne. Zwykle interfejs Implementation zapewnia
/// tylko prymitywne operacje , gdzie abstrakcja definiuje operacje
/// wysokiego poziomu bazujace na nich


class Implementation
{
public:
	virtual ~Implementation() {}
	virtual std::string operationImplementation() const = 0;
};

///Kazda konkretna implementacja odpowiada specyficznej platformie
/// i implementuje interfejs Implementation

class ConcreteImplementationA:
		public Implementation
{
	// Implementation interface
public:
	virtual std::string operationImplementation() const override
	{
		return  "ConcreteImplementationA: Here's the result on the "
				"platform A.\n";
	}
};

class ConcreteImplementationB:
		public Implementation
{
	// Implementation interface
public:
	virtual std::string operationImplementation() const override
	{
		return  "ConcreteImplementationB: Here's the result on the "
				"platform B.\n";
	}
};

///Abstrakcja definiuje interfejs dla czesci kontrolnej dla tych dwoch
///hierarchii klas. Zarzadza referencja do obiektu implementacji
///oraz deleguje cala prawdziwa prace do tego obiektu

class Abstraction
{
protected:
	Implementation* impl_;

public:
	Abstraction(Implementation* impl):
		impl_{impl}
	{}
	virtual ~Abstraction()
	{}

	virtual std::string operation() const
	{
		return  "Abstraction: Base operation with:\n" +
				this->impl_->operationImplementation();
	}
};

//mozna rozszerzyc klase abstrakcyjna bez zmiany klasy implementacyjnej

class ExtendedAbstraction:
		public Abstraction
{
public:
	ExtendedAbstraction(Implementation* impl):
		Abstraction{impl}
	{}
	// Abstraction interface
public:
	virtual std::string operation() const override
	{
		return  "ExtendedAbstraction: Base operation with:\n" +
				this->impl_->operationImplementation();
	}
};

//Za wyjatkiem etapu inicjalizacji ,gdzie obiekt Abstrakcji jest
//inicjalizowany jakims obiektem implementacji klient powinien
//polegac tylko na klasie abstrakcyjnej. Dzieki temu klient
//moze korzystac z wielu kombinacji implementacja-abstrakcja

void  clientCode(const Abstraction* abs)
{
	std::cout << abs->operation() << std::endl;
}

void b_main()
{
	Implementation* impl {new ConcreteImplementationA{}};
	Abstraction* abs {new Abstraction{impl}};
	clientCode(abs);
	std::cout << std::endl;
	delete impl;
	delete abs;

	impl = new ConcreteImplementationB{};
	abs = new ExtendedAbstraction{impl};
	clientCode(abs);

	delete impl;
	delete abs;

}
