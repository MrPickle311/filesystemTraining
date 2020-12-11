#pragma once

//wzorzec ten dzieli sie na kontenery oraz liscie
//Pudelko w pudelku

//Poznam go po tym ,ze wszystkie klasy naleza do jednej hierarchii
//Klasy deleguja zadania swoim dzieciom(podkomponentom) za posrednictwem
//klasy bazowej lub bazowego interfejsu hierarchii

///Wzorzec Kompozyt pozwala wykonywać działania
/// rekursywnie — po wszystkich komponentach drzewa.

///Największą zaletą tego podejścia jest to, że nie musimy się
/// przejmować konkretną klasą obiektów składających się na drzewo.
/// Nie musimy wiedzieć, czy obiekt jest prostym produktem, czy też
/// złożonym kontenerem.
/// Traktujemy wszystko w taki sam sposób, za pomocą takiego
/// samego interfejsu. Gdy wywołasz metodę, same obiekty przekażą
/// ją sobie dalej.

///Najmniejsze komponenty(liscie) wykonuja najwieksza ilosc pracy
/// gdyz nie maja komu jej zlecic

///Kontener(kompozyt) deleguje cala prace swoim podrzednym komponentom

///Klient moze za pomoca tego samego interfejsu pracowac na prostych
///elementach , jak i zlozonych

//swietne do zastosowan wielowatkowych

///Zastosowanie:
///1.Stosowanie wzorca Kompozyt ma sens tylko w przypadku, gdy
/// główny model twojej aplikacji/zbioru klas można przedstawić
/// w formie drzewa.
///2.Kod dzialajacy na grafach
///3.Gdy chce by kod kliencki traktowal zarowno proste jak i zlozone
/// elementy jednakowo

///Implementacja:
/// 1.Model aplikacji musi byc drzewiasty.Potem go rozdzielam na elementy
/// i kontenery
/// 2.Deklaracja wspolnego interfejsu komponentu
/// 3.Stworzyc klase lisc reprezentujaca proste elementy, moze byc wiele
/// klas.Dziedziczy komponent
/// 4.Stworzyc klase kontener reprezentujaca zlozone elementy.Moze
/// przechowywac inne elementy. Dziedziczy komponent
/// 5.Definicja metod pozwalajaca dodawac i usuwac elementy w kontenerze

///Zalety:
/// 1.Wygodny sposob obslugi struktur drzewiastych
/// 2. Spelnia Open/Closed Principle

///Wada:
/// Ustalenie wspolnego interfejsu dla klas o diametralnie roznych
/// funkcjonalnosciach moze byc bardzo trudne. W pewnych przypadkach
/// trzeba bardzo uogolnic interfejs komponentu ,co uczyni go trudniejszym
/// do zrozumienia

///Powiazania ze wzorcami
///1.Moge zastosowac wzorzec budowniczego by tworzyc zlozone drzewa
/// Kompozytowe dzieki mozliwosci zaprogramowania ich etapow konstrukcji
/// tak, aby odbywaly sie rekurencyjnie
/// 2.Lancuch zobowiazan czesto laczy sie z Kompozytem. W takim przypdaku
/// komponent-lisc otrzymuje zadanie ,moze je przekazac poprzez lancuch
/// nadrzednych komponentow az do korzenia drzewa obiektow
/// 3.Iterator moze sie przemieszczac po drzewie
/// 4.Wizytator moze smigac po drzewie
/// 5.Liscie mozna zaimplementowac jako Pylki
/// 6.Kompozyt i Dekorator maja podobne diagramy struktur poniewaz oba
/// bazuja na rekursywnej kompozycji w celu zorganizowania nieokreslonej
/// liczby obiektow.
/// Dekorator posiada tylko jeden element podrzedny. Dekorator przypisuje
/// dodatkowe obowiazki opakowywanemu obietkowi ,zas kompozyt zleca robote
/// innym. Dekorator moze sluzyc rozszerzeniu zachowania okreslonego obiektu w
/// drzewie Kompozytowym
/// 7.Prototyp pozwalana klonowanie zlozonych struktur Dekoratora lub
/// Kompozytu zamiast budowac je od 0

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

/**
 * Ta klasa bazowa deklaruje wspolne operacje dla dwoch prostych i dwoch
 * zlozonych obiektow kompozycji
 */

class Component
{
protected:
	Component* parent_;
	/**
	*Opcjonalnie ta baza moze deklarowac interfejs po to by ustawiac i
	* uzyskiwac dostep do rodzica w drzewiastej strukturze. Moze takze
	* zapewniac kilka domyslnych implementacji dla tych metod
	  */
public:
	virtual ~Component() {

	}

	void setParent(Component* parent)
	{
		this->parent_ = parent;
	}
	Component* getParent()
	{
		return  this->parent_;
	}
	/**
	  * W kilku przypadkach korzystne by bylo zdefiniowanie zarzadzania
	  * dziecmi w tej bazie.
	  */
	virtual void add(Component* compoment) {}
	virtual void remove(Component* compoment) {}

	///moge zapewnic metode , ktora pozwala kodowi klienckiemu wiedziec
	/// kiedy
	/// komponent moze tworzyc dzieci

	virtual bool isComposite() const
	{
		return false;//domyslnie nie mozna
	}

	//baza moze implementowac jakies domyslne zachowanie elementow
	virtual std::string operation() const = 0;
};

/**
  *Klasa Leaf reprezentuje ostatni element kompozycji , nie moze miec dzieci
  * Zwykle on odwala najwieksza robote
  */

class Leaf:
		public Component
{
	// Component interface
public:
	virtual std::string operation() const override
	{
		return "Leaf";
	}
};

/**
  *Klasa Composite reprezentuje zlozony zbior komponentow , ktore moga
  * miec dzieci. Praca jest wykonywana rekurencyjnie
  */

class Composite:
		public Component
{
protected:
	std::list<Component*> children_;

	// Component interface
public:
	//obiekty kompozytu moga dodawac lub odejmowac inne komponenty
	virtual void add(Component* compoment) override
	{
		this->children_.push_back(compoment);
		compoment->setParent(this);
	}
	//uzywaj smart pointerow
	virtual void remove(Component* compoment) override
	{
		children_.remove(compoment);
		compoment->setParent(nullptr);
	}
	virtual bool isComposite() const override
	{
		return true;
	}
	///Wykonanie zadania na kompozycie sprawia on ,ze on sam pobiera
	/// wyniki tej samej pracy od swoich dzieci (rekurencyjnie)
	virtual std::string operation() const override
	{
		std::string result;
		for(Component const* c: children_)
		{
			if(c == children_.back())
				result += c->operation();
			else result += c->operation() +'+';
		}
		return "Branch(" + result + ")";
	}
};

//Klient pracuje na jednym interfejsie niezaleznie od tego ,czy jest
//to jakis zlozony komponent ,czy prosty

void clientCode(Component* component)
{
	std::cout << component->operation() << std::endl;
}

void clientCode2(Component* component1,Component* component2)
{
	if(component1 == component2)
		component1->add(component2);

	std::cout << "Result" << component1->operation() << std::endl;
}

//Tutaj kod klienta wspiera proste komponenty Leaf

void c_main()
{
	Component* simple = new Leaf;
	std::cout << "Client: I've got a simple component:\n";
	clientCode(simple);
	std::cout << "\n\n";

	Component* tree = new Composite;
	Component* branch1 = new Composite;

	Component* leaf1 = new Leaf;
	Component* leaf2 = new Leaf;
	Component* leaf3 = new Leaf;

	branch1->add(leaf1);
	branch1->add(leaf2);

	Component* branch2 = new Composite;
	branch2->add(leaf3);

	tree->add(branch1);
	tree->add(branch2);

	std::cout << "Client: Now I've got a composite tree:\n";

	clientCode(tree);
	std::cout << "\n\n";

	std::cout << "Client: I don't need to check the components classes even when managing the tree:\n";

	clientCode2(tree,simple);

	std::cout << "\n";

	  delete simple;
	  delete tree;
	  delete branch1;
	  delete branch2;
	  delete leaf1;
	  delete leaf2;
	  delete leaf3;
}
