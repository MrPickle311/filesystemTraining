#pragma once

/* Definiuje szkielet algorytmu w klasie bazowej , ale pozwala podklasom nadpisać
 * pewne etapy tego algorytmu bez konieczności zmiany jego struktury
 */

/*Zastosowanie:
 * 1.Gdy chcę pozwolić klientom na rozszerzenie tylko niektórych etapów algorytmu,
 * ale nie całego , ani jego struktury
 * 2.Gdy mam wiele niemal identycznych algorytmów różniących się jedynie szczegółami
 * Wtedy modyfikacja algorytmu -> konieczność modyfikacji wszystkich klas
 */

/*Implementacja
 * 1.Podziel algorytm docelowy na etapy. Zobacz , które etapy będą wspólne , a
 * które można skonkretyzować
 * 2.Stworzyć abstrakcyjną klasę bazową,metodę szablonową i resztę. Do metody
 * szablonowej daj słowo final ,by nie móc jej nadpisać
 * 3.Zaimplementuj operacje domyślne, hooki i zadeklaruj metody abstrakcyjne
 * 4.Tworzenie wariantów algorytmów
 */

/*Zalety:
 * 1.Można pozwolić klientom nadpisać tylko niektóre partie dużego algorytmu
 * czyniąc go odporniejszym na szkody wskutek zmian poszczególnych jego części
 * 2.Można przenieść powtarzający się kod do klasy bazowej
 */

/*Wady:
 * 1.Szkielet algorytmu może stanowić ograniczenie
 * 2.Istnieje możliwość naruszenia Liskov Substitution Principle
 * 3.Metody szablonowe podczas rozrostu są trudne do utrzymania
 */

/*Powiazania:
 * 1.Jest to wyspecjalizowana metoda szablonowa. Moze stanowic
 *jeden z etapow metody szablonowej
 * 2.Metoda szablonowa polega na mechanizmie dziedziczenia: pozwala zmieniać części
 *aglorytmu rozszerzając je w podklasach. Strategia bazuje na kompozycji :
 *można zmienić częśc zachowania obiektu nadanie mu różnych strategii.Metoda działa
 *na poziomie klasy-> statyczna. Strategia -> kompozycja ->runtime
 */

#include <iostream>

/*Klasa Abstract Class definiuje metodę szablonową ,która zawiera szkielet dla
 * kilku algorytmów złożoną z wywołań abstrakcyjnych prymitywnych operacji
 */

/* Konkretne pochodne powinny implementować te operacje ,ale klasa Abstarct Class
 * może już sama w sobie definiować domyślne ich wersje
 */

class AbstractClass
{
protected:
	void baseOperation1() const
	{
		std::cout << "AbstractClass says: I am doing the bulk of the work\n";
	}
	void baseOperation2() const
	{
		std::cout << "AbstractClass says: But I let subclasses override some operations\n";
	}
	void baseOperation3() const
	{
		std::cout << "AbstractClass says: But I am doing the bulk of the work anyway\n";
	}
	//te operacje muszą być zaimplementowane przez pochodne
	virtual void requiredOperation1() const = 0;
	virtual void requiredOperation2() const = 0;
	/*Tutaj są tzw. hooki. Klasy mogą je nadpisać ,ale to nie jest obowiązkowe
	 * gdyż mają domyślną ( pustą ) implementację. Hooki zapewniają dodatkowe punkty
	 * rozszerzeń w istotnych miejsach algorytmu
	 */
	virtual void hook1() const {}
	virtual void hook2() const {}
public:
	void templateMethod() const //szkielet algorytmu
	{
		this->baseOperation1();
		this->requiredOperation1();
		this->requiredOperation2();
		this->hook1();
		this->requiredOperation1();
		this->baseOperation3();
		this->hook2();
	}
	virtual ~AbstractClass() {}
};

class ConcreteClass1:
		public AbstractClass
{
public:
	virtual ~ConcreteClass1() {}
	// AbstractClass interface
protected:
	virtual void requiredOperation1() const override
	{
		 std::cout << "ConcreteClass1 says: Implemented Operation1\n";
	}
	virtual void requiredOperation2() const override
	{
		 std::cout << "ConcreteClass1 says: Implemented Operation2\n";
	}
};

class ConcreteClass2:
		public AbstractClass
{
public:
	virtual ~ConcreteClass2() {}
	// AbstractClass interface
protected:
	virtual void requiredOperation1() const override
	{
		std::cout << "ConcreteClass2 says: Implemented Operation1\n";
	}
	virtual void requiredOperation2() const override
	{
		std::cout << "ConcreteClass2 says: Implemented Operation1\n";
	}
	virtual void hook1() const override
	{
		std::cout << "ConcreteClass2 says: Overridden Hook1\n";
	}
};

/*Kod klienta wywołuje metodę szablonową ,by wywołać algorytm. Kod klienta nie
 * musi wiedzieć jak konkretna klasa działa z obiektem ,tak długo jak pracuje
 * poprzez wspólny interfejs.
 */

void clientCode(AbstractClass* c)
{
	c->templateMethod();
}

void tm_main()
{
	std::cout << "Same client code can work with different subclasses:\n";
	ConcreteClass1* c1 {new ConcreteClass1};
	clientCode(c1);
	std::cout << "\n";
	std::cout << "Same client code can work with different subclasses:\n";
	ConcreteClass2* c2{new ConcreteClass2};
	clientCode(c2);
	delete  c1;
	delete  c2;
}
