#pragma once

///Wzorzec pozwalajacy na dodawanie obiektom nowych obowiazkow w sposob dynamiczny
///poprzez opakowanie ich w specjalne obiekty posiadajaca potrzebna funkcjonalnosc

///Obiekty mozna opakowac wieloktrotnie ,gdyz zarowno obiekt docelowy jak i dekoratory
///sa zgodne pod katem interfejsu. Wynikowy obiekt bedzie posiadal ulozona w formie stosu
///funkcjonalnosc wszystkich opakowan

//Jest szczegolnie obecny w strumieniach C++
//Dekoratora mozna poznac po metodach kreacyjnych lub konstruktorach przyjmujacych
//obiekty tej samej klasy lub interfejsu

///Zastosowanie:
/// 1.Gdy chce przypisywac dodatkowe obowiazki obiektom w trakcie dzialania progrmau
/// 2.Gdy rozszerzenie obowiazkow za pomoca dziedziczenia jest niepraktyczne lub
/// niemozliwe

///Implementacja:
/// 1.Moja aplikacja musi miec warstwowe przedstawienie w postaci podstawowego komponentu
/// 2.Ustalenie metod wspolnych dla podstawowego komponentu jak i warstw opcjonalnych
/// 3.Stworzenie klasy konkretnego komponentu/komponentow
/// 4.Stworzenie klasy bazowej dekoratora dziedziczac po komponencie. Dekorator bazowy musi
/// delegowac prace obiektowi opakowywanemu
/// 5.Stworzenie konkretnych dekoratorow.Konkretny dekorator musi wykonywać swoje
/// zadania przed lub po wywołaniu metody rodzica
/// 6.Kod kliencki tworzy dekoratory i sklada je wedlug swoich potrzeb

///Zalety:
/// 1.Mozna rozszerzyc zachowanie obiektu bez tworzenia jego podklasy
/// 2.Mozna dodawac lub usuwac obowiazki w czasie dzialania programu
/// 3.Mozliwe laczenie wielu zachowan
/// 4.Spelnia  Single Responsibility Principle

///Wady:
/// 1.Usuwanie nakladek jest trudne
/// 2.Trudno jest zaimplementowac dekorator tak ,by jego zachowanie bylo definiowane
/// kolejnoscia dodawania nakladek
/// 3.Kod wstepnie konfigurujacy nakladki moze wygladac brzydko

///Powiazania z innymi wzorcami:
/// 1.Adapter zmienia interfejs istniejacego obiektu ,zas Dekorator rozszerza go bez zmiany
/// interfejsu. Dekorator wspiera rekursywna kompozycje ,co nie jest mozliwe dla Adaptera
/// 2.Adapter wyposaza opakowany obiekt w inny interfejs, Pelnomocnik w taki sam, zas
/// Dekorator wprowadza rozszerzony interfejs
/// 3.Lancuch zobowiazan i Dekorator maja bardzo podobne struktury klas.Oba wzorce bazują na
/// rekursywnej kompozycji w celu przekazania obowiązku wykonania przez ciąg obiektów.
/// Ale istnieja roznice:
/// Obslugujacy Lancucha zobowiazan moga wykonywac dzialania niezaleznie od siebie.
/// Mogą również zatrzymać dalsze przekazywanie żądania na dowolnym etapie. Z drugiej strony,
///  różne Dekoratory mogą rozszerzać obowiązki obiektu zachowując zgodność
/// z interfejsem bazowym. Dodatkowo, dekoratory nie mają możliwości przerwania
/// przepływu żądania.
/// 4.Kompozyt i Dekorator maja podobne diagramy struktur poniewaz oba
/// bazuja na rekursywnej kompozycji w celu zorganizowania nieokreslonej
/// liczby obiektow.
/// Dekorator posiada tylko jeden element podrzedny. Dekorator przypisuje
/// dodatkowe obowiazki opakowywanemu obietkowi ,zas kompozyt zleca robote
/// innym. Dekorator moze sluzyc rozszerzeniu zachowania okreslonego obiektu w
/// drzewie Kompozytowym
/// 5.Prototyp pozwalana klonowanie zlozonych struktur Dekoratora lub
/// Kompozytu zamiast budowac je od 0
/// 6.Dekorator pozwala zmienic otoczke obiektu ,zas Strategia jej wnetrze
/// 7.Dekorator i Pelnomocnik maja podobne struktury ,ale inne cele. Oba wzorce bazuja
/// na zasadzie kompozycji - jeden obiekt deleguje czesc zadan innemu.Pelnomocnik dodatkowo
/// zarzadza cyklem zycia obiektu udostepniajacego jakas usluge , zas komponowanie Dekoratorow
/// lezy w gestii klienta

#include <iostream>
#include <string>

///Bazowy interfejs komponentu definiuje operacje ,ktore moga byc modyfikowane przez
///dekoratorow

class Component
{
public:
	virtual ~Component() {}
	virtual std::string operation() const = 0;
};

///Konkretne komponenty zapewniaja domyslna implementacje operacji. Te klasy moga
///istniec w duzej liczbie i w roznych kombinacjach

class ConcreteComponent:
		public Component
{
	// Component interface
public:
	virtual std::string operation() const override
	{
		return  "Concrete Component";
	}
};

/**
 *Bazowa klasa dekoratora ma ten sam interfejs jak inne komponenty. Ta klasa zapewnia
 * interfejs opakowan dla konkretnych dekoratorow. Domyslna implementacja opakowywania
 * moze zawierac pole z komponentem oraz domyslnie tylko inicjalizowac dekoratora
 */

class Decorator:
		public Component
{
protected:
	Component* comp_;
	// Component interface
public:
	Decorator(Component* comp):
		comp_{comp}
	{}
	//Dekorator deleguje cala prace do opakowywanego obiektu
	virtual std::string operation() const override
	{
		return this->comp_->operation();
	}
};

/**
  *Konkretne dekoratory moga wywolywac operacje opakowanych obiektow i modyfikowac
  * wyniki ich operacji
  */

class ConcreteDecoratorA:
		public Decorator
{
	//Konkretne dekoratory wywoluja implementacje klasy bazowej ,zamiast wywolywac
	//operacje funkcji bezposrednio.
public:
	ConcreteDecoratorA(Component* comp):
		Decorator{comp}
	{}
	// Component interface
public:
	virtual std::string operation() const override
	{
		return "ConcreteDecoratorA(" + Decorator::operation() +")";
	}
};

///Dekoratory moga wywolywac swoje zachowania zarowno przed jak i po wywolaniu
/// operacjidanego obiektu


class ConcreteDecoratorB:
		public Decorator
{
	//Konkretne dekoratory wywoluja implementacje klasy bazowej ,zamiast wywolywac
	//operacje funkcji bezposrednio.
public:
	ConcreteDecoratorB(Component* comp):
		Decorator{comp}
	{}
	// Component interface
public:
	virtual std::string operation() const override
	{
		return "ConcreteDecoratorB(" + Decorator::operation() +")";
	}
};

//Kod klienta dziala z wszystkimi obiektami uzywajacego interfejsu Component.
//To zapewnia niezalenzosc kodu klienta o konkretnych klas obiektow

void clientCode(Component* component)
{
	std::cout << "RESULT" << component->operation();
}

void d_main()
{
	Component* simple {new ConcreteComponent};
	std::cout << "Client: I've got a simple component:\n";
	clientCode(simple);

	//dekoratory moga opakowywac inne dekoratory

	Component* decorator1 {new ConcreteDecoratorA{simple}};
	Component* decorator2 {new ConcreteDecoratorB{decorator1}};
	std::cout << "Client: Now I've got a decorated component:\n";
	clientCode(decorator2);
	std::cout << '\n';

	delete simple;
	delete decorator1;
	delete decorator2;
}



































