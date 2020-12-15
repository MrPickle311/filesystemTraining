#pragma once

///Jezeli mam jakies publiczne pola/funkcje w klasie i jezeli dziedzicze
/// po niej w sposob protected/private ,to by uzyskac do nich odpowiedni
/// dostep private/protected ,to musze uzyc operatora using jak ponizej

class A
{
public:
	int x;
	int z;
	int h;
};

class B:
		private A
{
public:
	using A::z;
public:
	int y;
protected:
	using A::h;
};

void f1()
{
	B b;
	b.y = 2;
	b.z = 7;

}
