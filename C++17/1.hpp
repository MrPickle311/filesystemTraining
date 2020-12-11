#pragma once

#include <iostream>

struct x
{
	x()
	{
		std::cout << "x! \n";
	}
};

void F(x x1 = x{}, x x2 = x{})
{
	std::cout << " F !\n";
}

struct Y
{
	Y(x x1 = x{})
	{
		std::cout << " y!\n";
	}
};

template<typename T>
bool pred(T var )
{
	return var % 2 == 1;
}

bool op()
{
	return false and false and true;
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
		if (var % 2 == 1) ++count;
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

#include <sstream>

template<typename ... Args>
std::ostringstream prnt(std::string separator, Args... args)
{
	std::ostringstream os;
	auto prnt = [&](auto& arg) {os << arg << separator; };
	(prnt(args), ...);
	return os;
}

char tab[10]{};

template<size_t T,char* F>
void f1(int var)
{
	enum pip : int { primus = T , inter = 2 * T , pares = 4 * T };
	switch (var)
	{
	case pip::primus:
		for (auto& x : tab)
			x = '#';
		break;
	case pip::inter:
		for (auto& x : tab)
			x = '@';
		break;
	case pip::pares:
		for (auto& x : tab)
			x = '*';
		break;
	default:
		for (auto& x : tab)
			x = '.';
		break;
	}
}



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

