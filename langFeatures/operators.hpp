#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <type_traits>

//operatory new-delete

class XYZ
{
private:
	int a_;
	int b_;
	std::string c_;
public:
	XYZ(int a,int b,std::string c):
		a_{a},b_{b},c_{c}
	{}

	void prntAll() const
	{
		std::cout << a_ << " " << b_ << " " << c_;
	}

public:
	//operatory new-delete są domyślnie statyczne i dlatego nie można w nich użyć słowa this
	//new -> dostarcza odpowiednią liczbę bajtów
	//delete -> kasuje te bajty
	void* operator new(size_t n)
	{
		void* ptr {malloc(n)};
		if(ptr == 0x0)
			throw std::bad_alloc{};
		return ptr;
	}
	void operator delete(void* p,size_t n)
	{
		free(p);
	}
};

//operatory dwuargumentyowe oraz literały zdefiniowane przez użytkownika
//oraz operatory konwersji

class Complex
{
private:
	long double re_;
	long double im_;
public:
	constexpr Complex(long double re,long double im):
		re_{re},
		im_{im}
	{}
	constexpr Complex (const Complex& other):
		re_{other.re_},
		im_{other.im_}
	{}
	long double mod() const
	{
		return sqrt(re_*re_ + im_*im_);
	}
	Complex operator= (const Complex& other)
	{
		return other;
	}
	template<typename T>
	constexpr friend Complex operator+ (const T left,const Complex& right)
	{
		static_assert (std::is_arithmetic<T>{},"Musi to byc typ arytymetyczny!!!");
		return Complex{right.re_ + left,right.im_};
	}
	template<typename T>
	constexpr friend Complex operator+ (const Complex& left,const T right)
	{
		static_assert (std::is_arithmetic<T>{},"Musi to byc typ arytymetyczny!!!");
		return Complex{left.re_ + right,left.im_};
	}

	operator int () const
	{
		return sqrt(re_*re_ + im_*im_);
	}
	explicit operator bool () const
	{
		return re_ != 0;
	}
	explicit operator std::string () const
	{
		return std::to_string(mod());
	}
};

//operatory literałowe muszą być na zewnątrz
//jako argumenty można dać:
//1.long double
//2.unsinged long long
//3.char oraz inne typy charów np. wchar_t
//4.const char*
//5.przyjmujący parę argumentów (const char*,size_t)
constexpr Complex operator"" i(long double  im)
{
	return Complex{0,im};
}

//operator korzystajacy ze char* np. "asd"_j musi być zrobiony w ten sposób,dokładnie w ten
constexpr Complex operator"" _j(const char* a,size_t n) // musi być taki zestaw argumentów
{					//przed tym musi być podłogda
	return Complex{1,1};
}

template<char...chars>
constexpr Complex operator"" _k()
{
	(std::cout << ... << chars);
	return Complex{2,2};
}


void d(bool a,int b,std::string c)
{
	//...
}

void operator_test()
{
	XYZ* g{new XYZ{1,2,R"(sdsadasdas\n)"}};//dałem tutaj surowy łańcuch
	g->prntAll();
	delete g;

	std::cout << '\n';

	Complex h {2 + 2.3i + 2.2 + 2};
	Complex j = "fdsdfs"_j;
	Complex y = 5644_k; //tutaj puści tylko liczby,pokombinuj ze znakami jak będzie ci trzeba
	//d(h,h,h); tutaj nie zadziała ,gdyż operator std::string jest explicit
	d(h,h,static_cast<std::string>(h)); // musi być jawnie przez static_cast lub (std::string)h
	d(h,h,(std::string)h);
	std::cout << h.mod() <<'\n';
	std::cout << j.mod() << '\n';
}


