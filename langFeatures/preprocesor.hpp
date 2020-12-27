#pragma once

#ifdef __cplusplus
#include <iostream>
#endif

#ifdef __cplusplus
class x
{
public:
	int a;
};

class z:
		public x
{
public:
	int b;
};
#endif

//kompilacja warunkowa
#if false
void xd()
{

}
#endif

#ifdef __cplusplus
std::ostream& prnt(std::ostream& os)
{
	os << "sdsada";
	return os;
}

extern "C"
{
#endif
//kompilator C zobaczy tylko ten fragment
void* mchsprnrt(int** a,void *b)
{
	b = malloc(50);
	a[2][3] = 90;
	b = a[0];
	return b;
}

#ifdef __cplusplus
}
#endif

#if __has_include ("xd.cpp")

#endif

#if false
//dyrektywa #error przerywa kompilację i loguje błąd
#error "Bad libs"
#endif

//makro __LINE__ zwraca numer linii w postaci intigera
//makro __FILE__ zwraca const char*  z nazwą pliku w którym zostało to makro wywołane
//dyrektywa line zmienia wartości __LINE__ oraz __FILE__
#line 30
#line 56 "hello.cpp"

//#import jest tylko dla microsoftu

#define GOGO 123
void h()
{
	std::cout << GOGO;
}
//usuwanie makra
#undef GOGO

void u()
{
	//std::cout << GOGO;
}
