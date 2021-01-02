#pragma once
#include <iostream>
#include <vector>
using namespace std;
//Szablon może przyjmować parametry:
//1 - „typu typu”
//2 - Parametry wartościowe np. int(nie można zmiennoprzecinkowych)
//3 - wskaźniki do funkcji
//4 - parametry szablonowe „szablon typu”(szablony jako argumenty)

//1)
//Np.
//template<typename/class T>

//2)
//Argument tego typu może być:
//-całkowitoliczbowym wyrażeniem stałym
//-wskaźnikiem lub referencją do obiektu
//-nieprzeciążaonym wskaźnikiem do składowej
//-wskaźnikiem pustym

//Jako argument szablonu nie może wystąpić literał łańcuchowy

//Np.

template<class T,int max>
class Buffer
{
	//...
};

//wywołanie:
void o()
{
	int a {5};
	constexpr int b {6};
	Buffer<int,a> bff1; //błąd, powinno być wyrażenie stałe
	Buffer<int,4> bff; // ok
	Buffer<int,b> bff3; //ok
}


//Jeżeli chcę użyć łańcucha znaków (char*) jako argument szablonu to musi mieć stały rozmiar:

template<class T,char* label>
class Buffer2
{
	//...
};

//wywołanie:

void o2()
{
	Buffer2<int,"dsaasdaw"> bff; // błąd
	char lx2[] = "sdsds";
	Buffer2<int,lx2> x2; // ok
//	Parametr typowy można użyć jako typ w liście parametrów szablonu i dzięki temu np. mieć możliwość wykorzystania domyślnych argumentów
}

template<class T,T label = T{}>
class Buffer3{
	//...
};

int param = 3;
//wywołanie:
void o3()
{
	Buffer<int,4> bff;
	Buffer3<int> x2; // tutaj wartością label będzie 0
	Buffer3<int*,&param> c2; // label będzie wskaźnikiem na globalnej zmiennej param
}


//3)
//Wykorzystanie wskaźnika do funkcji jako argumentu szablonu

template<class Key,
		 class V,
		 bool(*cmp)(const Key&,const Key&)>
class map
{
public:
	map();
	//…
};

bool param1(const string& x ,const string& y){
	//...
}

map<string,int,param1> m;

//Wykorzystanie przekazania typu jako parametr typowy szablonu(bardziej w stylu C++). Tutaj używa się funktora ,który jest typem

template<class Key,class V,typename Compare = std::less<Key>> // domyślny
class	map2{															//obiekt
public:
	map2() {}						//używa domyślnego porównania
	map2(Compare c) : cmp{c} {}	//przesłania domyślne
	Compare cmp{};					//domyślne porównanie
};

//użycie lambdy jako parametru szablonu:

void o4()
{
	map<string ,int, [](const string& x,const string& y) const {return x<y;}> c3; //błąd

	auto cmp = [](const string& x,const string& y)->const bool {return x<y;};
	map<string ,int, decltype(cmp)> c3; //ok
}


//4)
//Np.

template<class T, template<class /*w tym miejscu określamy ile ma być*/ > class C>
class Param{					  /*parametrów typowych*/
	C<T> mems;
	C<T*> refs;
	//…
};

template<class T>
	using My_vec = std::vector<T>;

Param<int,My_vec /* ten szablon ma jeden parametr więc pasuje*/> x1;

template<class T>
class My_container{
	//...
};

Param<int,My_container> x2;


//Domyślne argumenty szablonów
//Np.

template<class T1 = int,class T2>
class Param1{		// błąd,domyślne argumenty muszą zaczynać się od prawej
	//...
};

template<class T1 = int , class T2 = double>
class Param2{		// ok
	//...
};

Param2<float> g; // g to Param1<float,double>

//Domyślne argumenty szablonów funkcji

//Np.
template<class T = string, class C = string>
T paramz(C arg){
//..
}

//wywołanie:

//typ argumentu szablonu da się wydedukować z argumentu funkcji np.

auto yx1 = paramz<string,double>(1.2);
auto yx2 = paramz<string>(1.2); // typ C to drogą dedukcji double
auto yx3 = paramz<>(1.2); // T to domyślnie string,a C double
auto yx4 = paramz(1.2); // w takim przypadku nie trzeba używać ostrych nawiasów

//								Specjalizacja

//Np.

template<class T>
class Paramo
{
	T* a;
public:
	Paramo() {}
	explicit Paramo(int);
	T& operator[](int i);
};

//przykład specjalizacji dla typu wskaźników do void

template<>
class Paramo<void*>// kompletna specjalizacja
{
	void** a;//nazwy zmiennych/funkcji a nawet ,,zawartość” klasy nie muszą być takie same
	void*& operator[](int i);
};

//użycie

Paramo<void*> s;

//Jest to specjalizacja kompletna tzn. jej użycie nie wymaga określenia , ani dedukcji parametru szablonu.

//Np. specjalizację przeznaczoną tylko i wyłącznie dla typów wskaźnikowych

template<typename T>
class Base
{
public:
	int a;
};

template<class T>
class Paramo<T*> : private Paramo<void*> { // specjalizacja częściowa
public:
	using Base = Paramo<void*>;
	Paramo() {}
	explicit Paramo(int i) : Base(i) {}
	// tutaj operator[] najpierw wyłuskuje obiekt poprzez * a potem przekazuje przez &
	T*& operator[](int i) { return reinterpret_cast<T*&>(Base::operator[](i)); }
};

Paramo<int*> a1; // <T*> to <int*> więc T to int
Paramo<int**> a2; // <T*> to <int**> więc T to int*

//Powyższa specjalizacja będzie wspólną implementacją dla wszystkich Param,które jako argument szablonu przyjmują wskaźnik

//Specjalizacja interfejsu

//np.
template<class T>
class complex{
public:
	complex& operator= (const complex&);
	complex<T>& operator= (const T&);

	template<class X>
		complex<T>& operator= (const complex<X>&);
};

template<>
class complex<float> {
public:
	complex(float re = 0.0, float im = 0.0);
	complex<float>& operator= (float);
	complex<float>& operator= (const complex<float>&);
};

//W C++ są dostępne tylko kompletne specjalizacje funckcji
