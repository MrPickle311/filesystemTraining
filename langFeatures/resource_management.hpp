#pragma once

#include <iostream>
#include <cstdio>
#include <mutex>

//RAII jest wzorcem projektowym, którego celem jest powiązanie czasu zajmowania zasobu z czasem życia obiektu – od jego konstrukcji po destrukcję.

//Problemy dotyczące zajmowania i zwalniania zasobów można rozwiązać przy użyciu obiektów klas z konstruktorami i destruktorami

//przykładowa nakładka FILE*
class File_ptr
{
private:
	FILE* p;
public:
	File_ptr(const char* n,const char* a ):
		p{fopen(n,a)}
	{
		if(p == nullptr)
			throw std::runtime_error{"File_ptr : nie można otworzyć pliu"};
	}
	File_ptr(const std::string& str,const char* a ):
		File_ptr{str.c_str(),a}
	{}

	explicit File_ptr(FILE* f)
	{
		if (f == nullptr)
			throw std::runtime_error{"File_ptr : przekazano nullptr"};
	}

	//cośtam

	~File_ptr()//Destruktor zostanie wywołany niezależnie od tego ,czy zostanie zgłoszony wyjątek ,czy zostanie osiągnięty koniec zakresu
	{		   //dzięki temu niezłapanie wyjątku w catch nie spowoduje memory-leak
		fclose(p);
	}

	operator FILE* ()
	{
		return p;
	}

};

//Kolejny przykład:

class buffer
{
private:
	std::byte* value_;
public:
	buffer(size_t size)
	{
		value_ = new(std::nothrow) std::byte[size];
		if(!value_)
			throw std::bad_alloc{};
	}

	~buffer()
	{
		delete[] value_;
	}

	void* get()
	{
		return value_;
	}
};

void bar(void*);//cośtam robi

void foo()
{
	try
	{
		buffer my_buff {100'000'000};
		bar(my_buff.get());
	}
	catch (std::bad_alloc& e)
	{
		//obsługa
	}
}


//Dobry konstruktor powinien albo tworzyć obiekt w pełni ,albo w przypadku problemów powinien przywracać stan sprzed jego wywołania
//Można to osiągnąć stosując technikę RAII dla składowych, czyli po prostu używanie składowych spełniających RAII ,same się zdemontują
//nie trzeba pisać dodatkowego kodu np.

class Locked_file_handle
{
	File_ptr p; //spełnia RAII
	std::unique_lock<std::mutex> lck; // spełnia RAII
public:
	Locked_file_handle(const char* x,std::mutex& m): //Wszystkie składowe spełniają RAII ,więc Locked_file_handle też spełnia , więc nie trzeba pisać dodatkowego kodu ogarniającego
		p{x,"rw"},																															//memory-leaki
		lck{m}
	{}
};

//Kontenery i obiekty STl spełniają RAII i nie powodują memory-leak
//Poziomy gwarancji STL
//1.Gwarancja podstawowa - zachowane niezmienniki oraz brak memory-leak
//2.Silna gwarancja - operacja zakończy się sukcesem albo nie wywoła żadnego efektu : dla kluczowych operacji (np. push_back, uninitialized_copy, insert)
//3,Gwarancja niezgłaszania wyjątku - dla prostych operacji np. swap, pop_back

//by to działało muszą być spełnione warunki:
// -destruktory własnych klas nie zgłaszają wyjątków
// -moje operacje nie powodują memory-leak
// -nie wprowadzam kontenerów w zły stan

//wyrażenia dzielą się na
//rvalue
//glvalue

//glvalues dzielą się na :
//lvalue
//xvalue

//rvalues dzielą się na :
//xvalues - wartości wygasające
//prvalues(Pure Right Values) - wartości właśnie otrzymane

//xvalues
//xvalues to wszystkie wyrażenia, których wynik jest typu rvalue referencja


std::string res()
{
	std::string res;
	//..
	return res;// res nie zostanie użyte po return ,przez co przeniesienie jest bezpieczne
}


//prvalues

void f()
{
	//są reprezentowane wyrażeniem wywołania jakiejś funkcji/konstruktora
	//są tymczasowe i nieprzechwycone są od razu niszczone
	//jest to po prostu bezpośrednia referencja do zwróconej wartości
	std::string res1 = std::string{};
	std::string r2 = res();

	//std::string& a = res();
	//za pomocą lvalue nie mogę tak zrobić ,gdyż lvalue odnosi się tylko do wartości
	//nazwanych
}

void s(std::string&& s)
{
	std::string a = s;
}

template <typename T>
constexpr T&& r_cast(T& value) noexcept
{
	return static_cast<T&&>(value);
}

void z()
{
	std::string b;
	//s(b); nie zadziała
	s(std::move(b));
	//zadziała , std::move wykonuje rzutowanie opisane szablonem powyżej, jednak robi to lepiej
}



















