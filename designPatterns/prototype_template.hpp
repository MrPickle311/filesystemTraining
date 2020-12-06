#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

//wzorzec w C++ jest od razu dostepny dzieki interfejsowi Cloneable
//wzorzec ten mozna poznac po metodzie copy(),clone() itd...

//pozwoli ci kopiowac istniejace obiekty bez uzalezniania twojego
//kodu od ich klas

///Zastosowanie:
/// 1.By moj kod nie byl zalezny od konkretnej klasy kopiowanego obietku
/// Wzorzec ten pozwala mojemu kodowi na prace z wszystkimi obiektami
/// wspierajacymi klonowanie za pomoca uogolnionego interfejsu
/// 2.By zredukowac ilosc podklas ,ktore roznia sie jedynie sposobem
/// inicjalizacji moich obiektow. Mnogosc i obecnosc obiektow i roznych
/// konfiguracjach

//Implementacja:
//1.

enum class Type : size_t
{
	Prototype_1,
	Prototype_2
};

//Przyklad klasy , ktora ma zdolnosc do klonowania
//Zobaczymy jak wartosci poszczegolnych pol roznych typow
//zostana sklonowane


class Prototype
{
protected:
	std::string prototype_name_;
	float prototype_field_;
public:
	Prototype(){}
	Prototype(std::string prototype_name):
		prototype_name_{prototype_name}
	{}
	virtual ~Prototype() {}
	virtual Prototype* clone() const = 0;
	virtual void method(float prototype_field)
	{
		this->prototype_field_ = prototype_field;
		std::cout << "Call method from : " << prototype_name_ <<
					 " with field : " << prototype_field_ << std::endl;
	}
};


//ConcretePrototype1 to podklasa Prototype implementujaca metode clone()
//W tym przykladzie wszystkie skladowe Prototype sa na stosie.
//Ale jesli umiescisz je na stercie,to pamietaj o ogarnieciu deep-copy

class ConcretePrototype1:
		public Prototype
{
	float concrete_prototype_field_1_;
public:
	ConcretePrototype1(std::string prototype_name,float concrete_prototype_field_):
		Prototype{prototype_name},
		concrete_prototype_field_1_{concrete_prototype_field_}
	{}

	//Zauwaz ,ze metoda clone() zwraca wskaznik do kopii ConcretePrototype1
	//To klient ma za zadanie zwolnic pamiec,ale lepiej uzywac intelignetnych
	//wskaznikow
	// Prototype interface
public:
	virtual Prototype* clone() const override
	{
		return new ConcretePrototype1{*this};
	}
};

class ConcretePrototype2:
		public Prototype
{
	float concrete_prototype_field_2_;
public:
	ConcretePrototype2(std::string prototype_name,float concrete_prototype_field_):
		Prototype{prototype_name},
		concrete_prototype_field_2_{concrete_prototype_field_}
	{}

	//Zauwaz ,ze metoda clone() zwraca wskaznik do kopii ConcretePrototype1
	//To klient ma za zadanie zwolnic pamiec,ale lepiej uzywac intelignetnych
	//wskaznikow
	// Prototype interface
public:
	virtual Prototype* clone() const override
	{
		return new ConcretePrototype2{*this};
	}
};

//W PrototypeFactory masz daw konkretne prototypy na kazda konkretna
//klase prototypowa, mozesz ich uzyc do klonowania

class PrototypeFactory
{
private:
	std::unordered_map<Type, Prototype*, std::hash<Type>> prototypes_;

public:
	PrototypeFactory()
	{
		prototypes_[Type::Prototype_1] = new ConcretePrototype1("Prototype_1",50.f);
		prototypes_[Type::Prototype_2] = new ConcretePrototype2("Prototype_2",60.f);
		//ale tutaj mozna dac calkiem inne [kryterium] , tak by np. wyszukiwalo po stringach
		//np. chce prototyp czerwonego guzika z calym jego setupem
	}

	//pamietaj ,by uzywac inteligentbych wskaznikow

	~PrototypeFactory()
	{
		delete prototypes_[Type::Prototype_1];
		delete prototypes_[Type::Prototype_2];
	}

	//Zauwaz tutaj ,ze ty tylko potrzebujesz okreslic typ prototypu ,ktory
	//jest ci potrzebny i metode klonujaca tego typu

	Prototype* createPrototype(Type type)
	{
		return prototypes_[type]->clone();
	}
};

void client_code(PrototypeFactory& prototype_factory)
{
	std::cout << "Lets create protype 1!" << std::endl;

	Prototype* prototype {prototype_factory.createPrototype(Type::Prototype_1)};
	prototype->method(90);

	delete prototype;

	std::cout << "\n";

	std::cout << "Lets create protype 2!" << std::endl;

	prototype = prototype_factory.createPrototype(Type::Prototype_2);

	prototype->method(10);

	delete  prototype;
}

void p_main()
{
	PrototypeFactory* prototype_factory = new PrototypeFactory;
	client_code(*prototype_factory);
	delete prototype_factory;
}






