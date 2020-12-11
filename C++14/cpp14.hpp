#pragma once

#include <iostream>
#include <string>
#include <type_traits>

//funkcja constexpr wykorzystuj�ca type_traits
template<class T,class F>
constexpr bool predicate()
{
	if (std::is_same<T, F>::value)
		return true;
	if (std::is_copy_assignable<T>::value)
		return true;
	return false;
}

template<typename T>
decltype(auto) operation(T var)
{
	//jak wida� w lambdach mo�na sobie przygotowa�
	//predefiniowane warto�ci na bazie wcze�niej dost�pnych
	auto lambda1 = [var = &var, var2 = var + var]()
	{
		return var + var2;
	};
	return lambda1();
}

[[deprecated("Ta funkcja sie zestarzala")]]
void func() 
{
	throw std::exception{};
}

void func(int a)
{

}

void fun2([[deprecated("X jest przestarzale !")]] int x, int y)
{
	std::cout << "!!!";
}

void fun2(size_t x, int y)
{
	std::cout << "!!!s";
}

template<class T,class F >
class Object_adapter
{
private:
	static_assert(std::is_class<T>(), "Parametr szablonu musi byc klasa");
	static_assert(std::is_class<F>(), "Parametr szablonu musi byc klasa");
private:
	//		szablon tej klasy sprawdza constexpr predykat i je�li
	//		zwr�ci on true ,to typem warto�ci var b�dzie T, je�li fa�sz ,to 
	typename std::conditional<predicate<T, F>(), T, F>::type var;
public:
	typedef typename std::conditional<predicate<T, F>(), T, F>::type type_t;
};

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
