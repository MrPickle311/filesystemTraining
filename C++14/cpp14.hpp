#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <type_traits>

////auto i decltype(auto)

auto g()
{
	return 0;
	//w deklaracji auto/decltype(auto) moge dac tylko jeden typ
	//inaczej kompilator zglosi blad
	//return 's';
}

//decltype(auto) przenosi wszystko , referencje , volatile itd...
//auto tylko surowy typ


//funkcja constexpr wykorzystuj¹ca type_traits
//tutaj trzeba uzyc decltype(auto) , gdyz gory trudno przewidziec
//jaki typ zostanie zwrocony, szczegolnie dla szablonu
//moze to byc nawet referencja albo wskaznik
template<typename T>
decltype(auto) operation(T var,T var2)
{
	//jak widaæ w lambdach mo¿na sobie przygotowaæ
	//predefiniowane wartoœci na bazie wczeœniej dostêpnych
	auto lambda1 = [var = &var, var2 = var + var,var3 {std::move(var2)}]()
	{	//w lambdzie mozna uzyc nastepujacych wyrazen inicjalizujacych
		return var + var2;
	};
	return lambda1();
}

///wyrazenia lambda

void lambdas()
{
	int a {2};

	//teraz mozna tworzyc uogolnione wyrazenia lambda , ktore przyjmuja
	//parametry typu auto ,czyli dowolnego typu
	auto lambda_template = [=](auto x,auto y)
	{
		return a != y;
	};

	std::cout << lambda_template(5,4);
	std::cout << lambda_template('2','3');
	std::string b{'2'};
	std::string c{"h"};

	//std::cout << lambda_template(b,c);
	//to nie zadziala niestety
}

///

///Funkcje constexpr
///Ograniczenia funkcji constexpr:
/// 1.nie mozna za pomoca niej przekazywac informacji asemblerowi
/// 2.nie mozna uzywac instrukcji goto
/// 3.nie mozna uzywac bloku try/catch
/// 4.nie mozna uzywac/tworzyc zmiennych typu nieliteralnego(niezdolnych do utworzenia zmiennych typu constexpr)
/// 5.nie mozna uzywac instrukcji majacych przydomek static lub thread_local
/// 6.nie mozna tworzyc definicji takich zmiennych constexpr,ktore nie maja inicjalizacji
/// 7.instrukcji modyfikujacej zmienna constexpr ,ktorej czas zycia rozpoczal sie w ramach funkcji constexpr
/// 8.nie mozna wywolywac niestatycznych funkcji skladowych , ktore nie sa constexpr
/// 9.funkcja constexpr nie moze byc virtual
/// 10.funkcja constexpr musi zwracac typ literalny

//ROZNICE MIEDZY CONSTEXPR ,A CONST

///Funkcje constexpr nie sa const!!!
/// int k() const oraz int k() zwracaja inne typy (const int oraz int)
/// lecz constexpr int k() oraz int k() zwracaja te same typy (int)!
/// nie mozna tworzyc takich przeciazen

constexpr size_t k(size_t x)
{
	size_t m{0};
	for(size_t i{0}; i < x*x; ++i)
	{
		size_t n {i % 4};
		switch (n)
		{
			case 0:
				++m;
				break;
			case 1:
				m *= 2;
				break;
			case 2:
				m += m*m;
				break;
			case 3:
				m += 9;
				break;
		}
	}
	return m;
}

constexpr void l()
{
	constexpr size_t g{k(5)};
}

template<class T,class F>
constexpr bool predicate()
{
	if (std::is_same<T, F>::value)
		return true;
	if (std::is_copy_assignable<T>::value)
		return true;
	return false;
}

///

template<class T,class F >
class Object_adapter
{
private:
	static_assert(std::is_class<T>(), "Parametr szablonu musi byc klasa");
	static_assert(std::is_class<F>(), "Parametr szablonu musi byc klasa");
private:
	//		szablon tej klasy sprawdza constexpr predykat i jeœli
	//		zwróci on true ,to typem wartoœci var bêdzie T, jeœli fa³sz ,to 
	typename std::conditional<predicate<T, F>(), T, F>::type var;
public:
	typedef typename std::conditional<predicate<T, F>(), T, F>::type type_t;
};

void fun2([[deprecated("X jest przestarzale !")]] int x, int y)
{
	std::cout << "!!!";
}

void fun2(size_t x, int y)
{
	std::cout << "!!!s";
}

[[deprecated("Ta funkcja sie zestarzala")]]
void func()
{
	throw std::exception{};
}

//a ta jest nowa
void func(int a)
{

}

class K
{
public:
	[[deprecated("przestarzala funkcja!")]]
	void drawing()
	{
		++drawer;
	}
	[[deprecated("przestarzaly skladnik!")]]
	int drawer;
};
