#pragma once

#include <iostream>
#include <string>

struct B
{

};

struct D:
		public B
{
	int a;
};

struct T
{
	virtual ~T() {}
	virtual void foo()
	{
		std::cout << "T foo\n";
	}
};

struct Q:
		public T
{
	// T interface
public:
	virtual ~Q() {}
	virtual void foo() override
	{}
	void special_foo()
	{
		std::cout << "Special foo \n";
	}
};

struct W:
		public T
{
	// T interface
public:
	virtual ~W() {}
	virtual void foo() override
	{}
	void better_special_foo()
	{
		std::cout << "better_special_foo\n";
	}
};

struct R:
		public W
{
	// T interface
public:
	virtual ~R() {}
	virtual void foo() override
	{}
	void giga_foo()
	{
		std::cout << "GIGA FOO\n";
	}
};

void callObj(T* ptr)
{
	if (Q* q = dynamic_cast<Q*>(ptr); q )
		q->special_foo();
	else if (W* w{dynamic_cast<W*>(ptr)}; w )
		w->better_special_foo();
	else ptr->foo();
	delete  ptr;
}

void callObjToReference(T* ptr)//tutaj już należy użyć bloków try_catch,co jest wolniejsze
{
	//ciężko za pomocą tego mechanizmu zrealizować wybieranie typów
	try
	{
		Q& q {dynamic_cast<Q&>(*ptr)};//typy muszą być na tym samym poziomie abstrakcji
		q.special_foo();
		return;
	}
	catch (std::bad_cast bc)
	{
		std::cout << "Not this type " << bc.what() << '\n';
	}
	try
	{
		W& w {dynamic_cast<W&>(*ptr)};
		w.better_special_foo();
		return;
	}
	catch (std::bad_cast bc)
	{
		std::cout << "Not this type " << bc.what() << '\n';
	}
	try
	{
		T& t {dynamic_cast<T&>(*ptr)}; //mimo ,że to ten sam typ , to mogę dostać wskaźnik ptr = nullptr!!
		t.foo();
	}
	catch (std::bad_cast bc)
	{
		std::cout << "Not this type " << bc.what() << '\n';
	}
}

///Downcastować można poprzez operatory konwersji : static_cast<>()
/// jednak static_cast<>() nie wymaga polimorficznego argumentu
/// static_cast<>() jest starsze i trochę bardziej niebezpieczne ,gdyż nie sprawdza ,czy konwersja przebiegła poprawnie
///
/// ! Obydwa operatory szanują przydomek const , nie można pozbyć się takim rzutowaniem przydomka const
/// ! Obydwa operatory szanują wszelakie prawa dostępu private,public,protected, czyli przykładowo nie mogę
/// zrzutować na prywatną bazę
///
///Lub poprzez dynamic_cast<>() ,czyli załóżmy ,że obiekt jest stworzony za pomocą konstruktora typu T
///oraz załóżmy ,że dziedziczy on z typu Q oraz W oraz derywuje w typ M ( M jest pochodną T )
/// Stawiam pytanie. Gdzie nie dostanę nullptr ?
///  Q				W
///    \           /
///     \		 /
///		  \    /
///			 T
///          |
///          |
///          M
///
///
///  T* t = new T;
/// Q* q = dynamic_cast<Q*>(t); // nie dostanę nullptr
/// W* w = dynamic_cast<W*>(t); // nie dostanę nullptr
/// M* m = dynamic_cast<M*>(t); //tutaj dostanę nullptr
///Czyli rzutujac tym operatorem na typ pochodny dla stworzonego typu daje nullptr

///By RTTI działało istnieje potrzeba definiowania wirtualnych destruktorów
/// lub innej wirtualnej funkcji

///Klasy powodujące niejednoznaczność

struct BaseClass
{
	virtual ~BaseClass() {}

};

struct DerivedCLass1:
		public BaseClass
{
	virtual ~DerivedCLass1() {}
};

struct DerivedCLass2:
		public BaseClass,
		public DerivedCLass1
{
	virtual ~DerivedCLass2() {}
};


void ambigious_test()
{
	DerivedCLass2* d2 {new DerivedCLass2};
	//BaseClass* b{dynamic_cast<BaseClass*>(d2)};//no i to nie ma prawa się skompilować
												 //gdyż nie wiadomo ,do którego BaseClass się
												 // odwołać , ale wirtualne dziedziczenie załatwia sprawę
	delete d2;
	BaseClass* b{new BaseClass};
	d2 = dynamic_cast<DerivedCLass2*>(b); //jednak to już jest możliwe ,gdyż jadę w dół
	//wiem z jakiej BaseClass jadę
}

void typeid_test()
{
	std::cout << "\ntypeid tests\n";
	DerivedCLass1* d{new DerivedCLass1};
	std::cout<< typeid (d).name() << '\n';

	//sprawdzam ,co się stanie gdy do typeid dam pusty wskaźnik

	try
	{
		BaseClass* b {nullptr};
		std::string s { typeid (b).name() };
		std::cout <<s << '\n';
	}
	catch (std::bad_typeid& ti)
	{
		std::cout << "Blad typeid  : " << ti.what() << '\n';
	}

}

void cast_operations()
{
	//B* ptr {new D{{},5}};
	//D* d{dynamic_cast<D*>(ptr)};
	//Takie rzutowanie będzie niemożliwe ,gdyż typ B
	//nie jest polimorficzny tzn. nie zawiera
	//przynajmniej jednej funkcji wirturalnej
	//wystąpi błąd kompilacji

	//spróbujmy odwrotnie

	D* ptr {new D{{},5}};
	B* b{dynamic_cast<B*>(ptr)};
	//ale takie rzutowanie jest możliwe i zachowuje się
	//tak samo jak static_cast

	//ponoć lepiej rzutować za pomocą dynamic_cast
	//niż dynamic_cast
	//dynamic_cast korzysta z RTTI ,lecz
	//static_cast nie używa RTTI

	//Ogólna zasada:
	//używam dynamic_cast gdy: poruszam się po drzewach
	//dziedziczenia
	//używam static_cast gdy: używam konstruktorów
	//konwertujących ,czy operatorów rzutowania

	std::cout << "Sending T \n";
	T* t {new T};
	callObj(t);

	std::cout << "Sending Q\n";
	Q* q {new Q};
	callObj(q);

	std::cout << "Sending W\n";
	W* w {new W};
	callObj(w);

	W* w2 {new W};
	R* t1 {dynamic_cast<R*>(w2)};
	if(t1 == nullptr)
		std::cout << "Wrong way!\n";

	//niby istnieje dynamic_cast<void*>(ptr) zwracający najbardziej pochodny typ,ale
	//nie jest to ani przyjemne , ani bezpieczne


	typeid_test();
	delete w2;
	delete t;
	delete q;
	delete w;
}
