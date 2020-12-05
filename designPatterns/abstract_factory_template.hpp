#pragma once
#include <string>
#include <iostream>

//obecne metody zwracajace obiekt fabryczny ,ktory potem sluzy
//tworzeniu konkretynych komponentow


//Kazdy odrebny produkt z jakiejs rodziny powinien impelementowac
//jakis bazowy interfejs, kazdy wariant tego produktu powinien
//rowniez implementowac ten interfejs

///Zastosowanie
/// - gdy kod ma dzialac na produktach roznych rodzin ,
/// ale nie chce ,by scisle zalezal od konkretnych klas
/// ,zeby dzialal na abstrakcji,projektujac aplikacje
/// moge nie znac konkretnych klas

//gdy jakas klasa ma do czynienia z wieloma typami produktow
//warto byc moze zebrac jej metody wytworcze i umiescic
//je w osobnej klasie fabrycznej , lub zaimplementowac pelna
//fabryke abstrakcyjne za pomoca nich

///jak zaimplementowac?
///1. Tworzenie mapy produktow poszczegolnych typow z uwzglednienem
/// wariantow w jakich moga one byc dostepne
/// 2.Dla kazdego typu produkty zaimplementuj abstrakcyjny interfejs
/// konkretne klasy powinny implementowac ten bazowy interfejs
/// 3. Deklaracja interfejsu fabryki abstrakcyjnej
/// 4.Konkretyzacja bazy abstrakcyjnej dla konkretnych
/// produktow oraz ich rodzin
/// 5.Gdzies w programie nalezy umiescic kod kreujacy
/// konkretna fabryke ,zalezna od jakis warunkow. Potem
/// nalezy przekazac ten obiekt fabryczny kazdej klasie
/// ,ktorej zadaniem jest kontrukcja produktow
/// 6.Zamienic wszystkie bezposrednie wywolania konstruktorow
/// produktow na odwolania do metod fabryk abstrakcyjnych

//Zalety:
//1.Zyskuje pewnosc ,ze produkty z jednej rodziny
//sa ze soba kompatybilne
//2. Zapobiegniecie sprzegniecia sie kodu klienkciego z
//konkretnymi produktami
//3.Spelniona zasada Single Responsibility Principle
//4.Spelniona zasada Open/Closed Principle

//Wada:
//Mozlwiosc komplikacji kodu ,duzo interfejsow

///Powiazania z innymi wzorcami
/// 1. Projekty stosujace metode wytworcza ewuluuja  stopniowo
///w Fabryke abstrakcyjna,prototyp lub budowniczego
/// 2. Budowniczy koncentruje sie na szczegolowym tworzeniu
/// jednego obiektu , gdzie fabryka abstrakcyjna specjalizuje
/// sie w tworzeniu rodzin spokrewnionych ze soba obiektow
/// 3.Fabryka abstrakcyjna moze sluzyc jako dobra alternatywa do
/// fasady gdy chcesz zrobic, to ukrycie przed kodem klienckim
/// procesu tworzenia obietkow
/// 4.Fabryka abstrakcyjna moze byc stosowana wraz z mostem.
/// Takie sparowanie jest uzyteczne ,gdy niektore abstrakcje
/// zdefiniowane przez most moga wspoldzialac wylacznie z
/// okreslonymi implementacjami. W tym przypadku fabryka moze
/// hermetyzowac te relacje i ukryc zawilosci przed kodem
/// klienckim
/// 5.Wszystkie fabryki,buodowniczowie ,prototypy mozna
/// implementowac jako singletony


//ROZNICE MIEDZY METODA WYTWORCZA!!!
//w metodzie wytworczej jawnie sie okresla typ tworzonego
//obiektu ,gdzie w fabryce abstrakcyjnej kod kliencki
//operuje na abstrakcyjnych obiektach i fabrykach

class AbstractProductA
{
public:
	virtual ~AbstractProductA() {}
	virtual std::string usefulFunctionA() const = 0;
};

//Konkretne produkty sa tworzone poprzez odpowiadajace im
//fabryki

class ConcreteProductA1 :
		public AbstractProductA
{
	// AbstractProductA interface
public:
	virtual std::string usefulFunctionA() const override
	{
		return "This is product A1 !";
	}
};

class ConcreteProductA2:
		public AbstractProductA
{
	// AbstractProductA interface
public:
	virtual std::string usefulFunctionA() const override
	{
		return "This is product A2 !";
	}
};

//Tutaj jest bazowy interfejs innego produktu. Wszystkie
//produkty moga wspoldzialac ze soba nawzajem , ale
//prawidlowa wspolpraca jest mozliwa tylko pomiedzy produktami
//tego samego wariantu

class AbstractProductB
{
	//produkt wykonuje jakies swoje zadania
public:
	virtual ~AbstractProductB (){}
	virtual std::string usefulFunctionB() const = 0;

	//ale moze wspolpracowac z AbstractProductA
	//Fabryka abstrakcyjna gwarantuje to ,ze wszystkie
	//produkty , ktore tworzy sa tego samego wariantu,
	//a zatem sa kompatybilne

	virtual std::string anotherUsefulFunctionB(const AbstractProductA& collaborator) const = 0;
};

//konkretyzacje

class ConcreteProductB1:
		public AbstractProductB
{
	// AbstratProductB interface
public:
	virtual std::string usefulFunctionB() const override
	{
		return "This is product B1! ";
	}

	//Ten wariant AbstratProductB czyli ConcreteProductB1
	//jest w stanie dobrze pracowac jedynie z ConcreteProductA1
	//ale nalezy zauwazyc ,ze przyjmuje instancje AbstractProductA

	virtual std::string anotherUsefulFunctionB(const AbstractProductA& collaborator) const override
	{
		std::string const res {collaborator.usefulFunctionA()};
		return  "The result of the B2 collaborating with ( " + res + " )";
	}
};

class ConcreteProductB2:
		public AbstractProductB
{
	// AbstractProductB interface
public:
	virtual std::string usefulFunctionB() const override
	{
		return  "This is product B2 ! ";
	}
	virtual std::string anotherUsefulFunctionB(const AbstractProductA& collaborator) const override
	{
		std::string const res {collaborator.usefulFunctionA()};
		return  "The result of the B2 collaborating with ( " + res + " )";
	}
};

//Interfejs fabryki abstrakcyjnej deklaruje zestaw metod
//zwracajacych rozne abstrakcyjne produkty ,ktore nazywa sie
//rodzina i sa zwiazane wysokopoziomowymi zaleznosciami
//Produkty jednej rodziny wspolpracuja z soba,
//prdukty z innych wariantow nie sa kompatybilne

class AbstractFactory
{
public:
	virtual ~AbstractFactory() {}
	virtual AbstractProductA* createProductA() const = 0;
	virtual AbstractProductB* createProductB() const = 0;
};


//interfejs fabryki abstrakcyjnej produkuje rodzine produktow
//ktora nalezy do pojedynczego wariantu. Fabryka gwarantuje
//to ,ze zwracane produtkty beda kompatybilne
//metody konkretnych fabryk zwracaja produkt abstrakcyjny
//gdy wewnatrz funckji zwrontej jest tworzony konkretny
//produkt

class ConcreteFactory1 :
		public AbstractFactory
{
	// AbstractFactory interface
public:
	virtual AbstractProductA* createProductA() const override
	{
		return new ConcreteProductA1{};
	}
	virtual AbstractProductB* createProductB() const override
	{
		return new ConcreteProductB1{};
	}
};

class ConcreteFactory2:
		public AbstractFactory
{
	// AbstractFactory interface
public:
	virtual AbstractProductA* createProductA() const override
	{
		return new ConcreteProductA1{};
	}
	virtual AbstractProductB* createProductB() const override
	{
		return  new ConcreteProductB2{};
	}
};

//kod kliencki pracuje z fabrykami i produktami tylko poprzez
//abstrakcyjne produkty. Pozwala to przekazywac jakakolwiek
//fabryke lub produkt do kody klienckiego bez deenkapsulacji

void clientCode(const AbstractFactory& factory)
{
	AbstractProductA const* product_a {factory.createProductA()};
	AbstractProductB const* product_b {factory.createProductB()};
	std::cout << product_b->usefulFunctionB() << std::endl;
	std::cout << product_b->anotherUsefulFunctionB(*product_a) << std::endl;
	delete  product_a;
	delete  product_b;
}

void af_main()
{
	std::cout << "Factory 1"<< std::endl;
	ConcreteFactory1* f1 {new ConcreteFactory1{}};
	clientCode(*f1);
	delete f1;
	std::cout << "Factory 2" << std::endl;
	ConcreteFactory2* f2 {new ConcreteFactory2{}};
	clientCode(*f2);
	delete f2;
}



