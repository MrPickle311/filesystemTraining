#pragma once

//Pozwala oddzielić algorytmy od obiektów na ,których pracują

/*Interfejs Visitor deklaruje zbiór metod odwiedzających ,które odpowiadają do
 * konkretnych klas-komponentów.Sygnatura metody odwiedzającej pozwala wizytatorowi
 * zidentyfikować konkretną klasę-komponent z ,którą ma do czynienia
 */

/*Zastosowanie:
 * 1.Gdy istnieje potrzeba wykonania jakiegoś działania na wszystkich elementach
 * złożonej struktury elementów
 * 2.Pozwala uprzątnąć logikę biznesową czynności pomocniczych
 * 3.Gdy jakieś zachowanie ma sens tylko w konteksćie klas wchodzących w skład
 * hierarchii klas ,ale nie wszystkich. Jeśli mam klasę ,która nie ma szczególnego
 * zachowania ,to dla niej pozostawiam pustą metodę accept()
 */

/*Implementacja:
 * 1.Deklaracja interfejsu wizytatora → metody dla komponentów
 * 2.Deklaracja interfejsu elementu , lub dodanie metody accept() do istniejącej
 * klasy bazowej
 * 3.Implementacja metod accept()
 * 4.Wizytatorzy musza być świadomi klas-komponentów → praca wyłącznie za pomocą
 * interfejsu wizytatora i komponentu
 * 5.Jeśli potrzebuję dostępu do prywatnych pól klasy to dobrym pomysłem jest
 * zagnieżdżenie klasy wizytatora
 * 6.Klient tworzy wizytatorów i komponenty
 */

/*Zalety:
 * 1.Spełnia Open/Closed Principle
 * 2.Spełnia Single Responsibility Principle
 * 3.Możliwość zbierania cennych informacji od konkretnych obiektów
 */

/*Wady:
 * 1.Trzeba aktualizować wszystkich wizytatorów zawsze ,gdy hierarchia elementów
 * zyskuje nową klasę ,lub ją traci
 * 2.Wizytatorzy mogą nie mieć dostępu do prywatynych danych klasy
 */

/*Powiązania:
 * 1.Wizytator można traktować jako potężniejszą wersję Polecenia.
 * Jego obiekty(wizytatora) mogą wykonywać różne polecenia na obiektach różnych klas
 * 2.Wizytator może wykonywać działania na drzewie Kompozytowym
 * 3.Połączenie Wizytatora z Iteratorem może służyć sekwencyjnemu przeglądaniu elementów
 * złożonej struktury danych i wykonaniu na nich jakiegoś działania ,nawet jeśli te elementy
 * są obiektami różnych klas
 */

#include <iostream>
#include <string>
#include <array>

class ConcreteComponentA;
class ConcreteComponentB;

class Visitor
{
public:
	virtual void visitComponentA(const ConcreteComponentA* element) const = 0;
	virtual void visitComponentB(const ConcreteComponentB* element) const = 0;
};

/*Interfejs Compoment deklaruje metodę accept() przyjmującą bazę wizytatorów*/

class Component
{
public:
	virtual ~Component() {}
	virtual void accept(Visitor* visitor) const = 0;
};

/*Każdy konkretny komponent musi implementować accept() by przyjąć wizytatora*/

class ConcreteComponentA:
		public Component
{
	// Component interface
public:
	/*Zauważ ,że tutaj wywołujemy visitComponentA ,który pasuje do bieżącej nazwy
	 * klasy. To pozwala dowiedzieć się wizytatorowi z jaką klasą pracuje.
	 */
	virtual void accept(Visitor* visitor) const override
	{
		visitor->visitComponentA(this);
	}
	/*Konkretne komponenty mogą mieć specjalne metody ,które nie istnieją w ich
	 * bazowej klasie lub interfejsie. Wizytator jest ciągle w stanie używać tych
	 * metod odkąd jest świadom konkretnej klasy komponentu.
	 */
	std::string ExclusiveMethodOfConcreteComponentA() const
	{
	   return "A";
	}
};

class ConcreteComponentB:
		public Component
{
	// Component interface
public:
	virtual void accept(Visitor* visitor) const override
	{
		visitor->visitComponentB(this);
	}
	std::string SpecialMethodOfConcreteComponentB() const
	{
		return "B";
	}
};

/*Konkretni wizytatorzy implementują kilka wersji tego samego algorytmu ,który może
 * pracować z wszystkimi komponentami
 */

class ConcreteVisitor1:
		public Visitor
{
public:
	virtual ~ConcreteVisitor1(){}
	// Visitor interface
public:
	virtual void visitComponentA(const ConcreteComponentA* element) const override
	{
		std::cout << element->ExclusiveMethodOfConcreteComponentA() << " Concrete Visitor1\n";
	}
	virtual void visitComponentB(const ConcreteComponentB* element) const override
	{
		std::cout << element->SpecialMethodOfConcreteComponentB() << " Concrete Visitor1\n";
	}
};

class ConcreteVisitor2:
		public Visitor
{
public:
	virtual ~ConcreteVisitor2(){}
	// Visitor interface
public:
	virtual void visitComponentA(const ConcreteComponentA* element) const override
	{
		std::cout << element->ExclusiveMethodOfConcreteComponentA() << " Concrete Visitor2\n";
	}
	virtual void visitComponentB(const ConcreteComponentB* element) const override
	{
		std::cout << element->SpecialMethodOfConcreteComponentB() << " Concrete Visitor2\n";
	}
};


/*Kod klienta może odpalić operacje na dowolnym zbiorze elementów nie znając ich typu
 * Operacja accept() kieruje wywołaniem do poszczególnej operacji w obiekcie wizytatora
*/

void clientCode(std::array<const Component*,2> c,Visitor* v)
{
	for(auto comp: c)	comp->accept(v);
}


void v_main()
{
	std::array<const Component*,2> c {new ConcreteComponentA,new ConcreteComponentB};
	ConcreteVisitor1* v1 {new ConcreteVisitor1};
	clientCode(c,v1);
	ConcreteVisitor2* v2{new ConcreteVisitor2};
	clientCode(c,v2);

	for(auto comp:c) delete comp;
	delete v1;
	delete v2;
}

