#pragma once
#include <iostream>
#include <sstream>

//ogolna skladnia fold_expressions
//op moze byc jednym z 32 operatorow dwuargumentowych

//(args bin_op ... bin_op start_expression/start_value )
//(arg_1 bin_op (arg_2 bin_op (... (arg_n-1bin_op (arg_n bin_op start_expression/start_value) )))

//(start_expression/start_value op .. op args)
// (((((start_expression/start_value op arg_1) op arg_2 ) op arg_3) op ...) op arg_n)

// (... op args)
// ((((arg_1 op arg_2) op arg_3) op ...) op arg_n)

// (args op ...)
// (arg_1 op (... op (arg_n-1 op arg_n)))))

//lub wywolywanie wyrazen po przecinkach
// (X.op(args),...)
// (X.op(arg_1),X.op(arg_2),...)

template<typename... Args>
auto right_sum(Args... args)
{
	//sumowanie wszystkich argumentow
	return (args + ...);
	//zostanie rozlozone do nastepujacej postaci
	//(arg_1 + (arg_2 + (... (arg_n-1 + arg_n))))
	//jest to harmonijka skladana w prawo gdyz nawiasy narastaja z prawej strony
}

template<typename... Args>
auto left_sum(Args... args)
{
	//sumowanie wszystkich argumentow
	return (... + args);
	//zostanie rozlozone do nastepujacej postaci
	//((((arg_1 + arg_2) + ...) arg_n-1) + arg_n)
	//jest to harmonijka skladana w lewo gdyz nawiasy narastaja z lewej strony
}

template<typename... Args>
void prints(Args... args)
{
	(std::cout				<< ... << args) << std::endl;
//  wyrazenie poczatkowe    op ... op args
//np. dla prints(1,2,3) kompilator zbuduje takie wyrazenie
	// (((std::cout << 1) << 2) << 3)    << std::endl;
}

template<typename ... Args>
auto check(Args... args)
{
	return (... && args);
}

template<typename ... Args>
auto get_odd_nmbrs_count(Args... args)
{
	size_t count{ 0 };
	auto pred = [&](auto& var, size_t& count)
	{
		if (var % 2 == 1)
			++count;
	};
	(pred(args,count), ...);
	return count;
}

template<typename T,typename ... Args>
void func(T var, Args& ... args)
{
	auto l = [&](auto& arg, T& var) {arg = var; };
	(l(args,var), ...);
}

template<typename ... Args>
std::ostringstream prnt(std::string separator, Args... args)
{
	std::ostringstream os;
	auto prnt = [&](auto& arg) {os << arg << separator; };
	(prnt(args), ...);
	return os;
}

///////dodawanie stalych jako parametr szablonu czyli template<auto>////////////////

//tak trzeba bylo robic przed C++17 ,by przekazac stala
// np. doSth<int,4>();
// jakis_typ a;
// doSth<decltype(&a),&a>();
template<typename T,T constans>
void doSth()
{
	//operacje
}

//a teraz tak mozna robic
// doSth2<5>();
// jakis_typ a;
// doSth<&a>();
template<auto constans>
void doSth2()
{
  //operacje
}

//tutaj przyjmie alba stala albo obiekt klasy
//Constans nie jest typem ,ale stala wartoscia!
template<auto Constans>
struct Class
{
	using T = decltype(Constans);
	T var = Constans;
	T vars[Constans];
	Class() {
		//std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	void fill_tab(T var)
	{
		for (auto& x : vars)
			x = var;
	}
	void prnt_tab(size_t count)
	{
		if (count < Constans)
			for (size_t i = 0; i < count; ++i)
				std::cout << vars[i] << " ";
		else for (auto& x : vars)
			std::cout << x << " ";
	}
};

///Dowolna ilosc stalych typu int,double itd...
template<auto ... constances>
double sum(...)
{
	return (... + constances);
}
