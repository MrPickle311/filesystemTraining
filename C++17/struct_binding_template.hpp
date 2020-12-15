#pragma once

#include <vector>
#include <string>
#include <array>
#include <iostream>


//Przystosowanie OBCEJ wlasnej klasy (z polami prywatnymi) do obslugi dowiazan

class ExternalClass
{
private:
	std::string var1_;
	char chr_;
	double nmbr_;
public:
	ExternalClass(std::string var1,char chr,double nmbr):
		var1_{var1},
		chr_{chr},
		nmbr_{nmbr}
	{}
	std::string var1() const
	{
		return var1_;
	}

	double nmbr() const
	{
		return nmbr_;
	}
};

//nie mam dostepu do tej klasy ,wiec funkcja get musi byc globalna
template<size_t N>
decltype (auto) get(ExternalClass c)
{
	static_assert (N == 1 || N == 0,"Musi byc rowne 1 lub 0 !!!");
	if constexpr (N == 0)
		return c.var1();
	else if constexpr (N == 1)
		return c.nmbr();
};

//potrzebne specjalizacje
template<>
struct std::tuple_size<ExternalClass>
{
	static size_t const value = 2;
};

template<>
struct std::tuple_element<0,ExternalClass>
{
	using type = std::string;
};


template<>
struct std::tuple_element<1,ExternalClass>
{
	using type = double;
};

void externalOperations()
{
	ExternalClass c{"23",'b',5.0};

	auto& [a,b] = c;
	a = "2;";
	b = 8;
	//nie mozna modyfikowac tej klasy
	//nie wierzysz ? to odpal ten programik
	std::cout << a << " " << b << std::endl;
	std::cout << c.var1() << " " << c.nmbr() << std::endl;
}

//Przystosowanie mojej wlasnej klasy (z polami prywatnymi) do obslugi dowiazan

//Potrzebuje
//1.getterow
//2.Potrzebuje specjalizacji klasy std::tuple_size<K> , gdzie trzeba umiescic informacje
//o tym ,ze jest K skladnikow do pobrania
//3.Potrzebuje specjalizacji kilku wersji specjalizacji szablonu klasy std::tuple_element
//na wypadek pracy z kazdym konkretnym skladnikiem nr N naszej klasy

class DataClass
{
private:
	std::string var1_;
	char chr_;
	double nmbr_;
public:
	DataClass(std::string var1,char chr,double nmbr):
		var1_{var1},
		chr_{chr},
		nmbr_{nmbr}
	{}
	std::string var1() const
	{
		return var1_;
	}

	double nmbr() const
	{
		return nmbr_;
	}

	///taka funkcja musi istniec
	template<size_t N>
	decltype (auto) get()
	{
		if constexpr (N == 0)
			return (var1_);
		else if (N == 1)
			return (nmbr_);
	}
};

///potrzebne specjalizacje
template<>
struct std::tuple_size<DataClass>
{
	static size_t const value = 2;
};

#define manually false

#if manually == true
//sami robimy dowiazania

template<>
struct std::tuple_element <0,DataClass>
{
	using type = std::string;
};

template<>
struct std::tuple_element<1,DataClass>
{
	using type = double;
};

#else
///AUTOMATYCZNIE
template<size_t N>
struct std::tuple_element <N,DataClass>
{
	using type = decltype (std::declval<DataClass>().get<N>());
	//declval po prostu zmienia typ T na referencje do r-wartosci typu T
};
#endif

void implicitOperations()
{
	//Przystosowanie mojej wlasnej klasy (z polami prywatnymi) do obslugi dowiazan
	DataClass dc{"xd",'c',2.6};

	auto& [a,b] = dc;

	b = 5.0;
	std::cout << b << std::endl;
	std::cout << dc.nmbr() << std::endl;

}

			//Sieganie do pol struktur/klas z publicznymi polami


struct DataStruct
{
	//wszystkie skladniki niestatyczne musza byc publiczne
	std::string var1_;
	double var2_;

	static bool pred_;
	int bitF_ : 4;
	bool info_;
};

//inicjalizacja
bool DataStruct::pred_ = true;



DataStruct getStructObj()
{
	return DataStruct{"xd",5.6,2,false};
}

void explicitOperations()
{
	//Sieganie do pol struktur/klas z publicznymi polami

	DataStruct d1{"var",6.7,3,true};
	auto [a,b,c,d] = d1;//tutaj ilosc dowiazan musi byc taka sama jak ilosc publicznych niestatycznych pol klasy
			a += " : 2";
			b = 6.0;
			d = false;
	//Pracujemy tutaj na kopii obiektu!!!
	std::cout << "Kopia : "  << a << std::endl;
	std::cout << "Oryginal : " <<d1.var1_ << std::endl;

	//tutaj pracujemy na oryginale
	auto& [e,f,g,h] = d1;
			e += " : 2";
			f = 6.0;
			h = false;
	std::cout << "Niby kopia : "  << e << std::endl;
	std::cout << "Oryginal : " <<d1.var1_ << std::endl;

	//auto& [m,n,b,v] = getStructObj();
	//to sie nie skompiluje ,gdyz nastepuje tutaj lewostronna referencja do r-wartosci bez  ,czego nie mozna robic

	auto [m,n,u,v] = getStructObj();

	//to zadziala ,gdyz jest to lewostronna referencja z przydomkiem const
	const auto& [q,w,p,r] = getStructObj();

	//moge tez uzyc prawostronnej referencji,czyli referencji do wartosci chwilowej
	auto&& [l,k,j,x] = getStructObj();


	if (auto&& [str,dbl,inti,boolean] = getStructObj();!boolean)
			std::cout << "\nTRUE!\n" << std::endl;

}







