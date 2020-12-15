#pragma once

#include <iostream>
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
decltype(auto) operation(T var)
{
	//jak widaæ w lambdach mo¿na sobie przygotowaæ
	//predefiniowane wartoœci na bazie wczeœniej dostêpnych
	auto lambda1 = [var = &var, var2 = var + var]()
	{
		return var + var2;
	};
	return lambda1();
}

///wyrazenia lambda

void lambdas()
{
	int a {2};

	//teraz mozna tworzyc uogolnione wyrazenia lambda , ktore przyjmuja
	auto lambda_template = [=](auto x,auto y)
	{
		return a < y;
	};
}

///

template<class T,class F>
constexpr bool predicate()
{
	if (std::is_same<T, F>::value)
		return true;
	if (std::is_copy_assignable<T>::value)
		return true;
	return false;
}

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
