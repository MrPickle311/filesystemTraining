#pragma once

#include <map>
#include <string>
#include <iostream>
#include <tuple>
#include <type_traits>

class set
{
private:
	std::string str_;
	double dbl_;
	int16_t int_;
public:
	char chr_;
	uint8_t uint_;
	double* ptr_;
public:
	set(std::string str,double dbl,int16_t nt,char chr,uint8_t uint,double* ptr):
		str_{str},
		dbl_{dbl},
		int_{nt},
		chr_{chr},
		uint_{uint},
		ptr_{ptr}
	{}
private:
	std::string get_str() const
	{
		return str_;
	}
public:
	template<std::size_t N>
	decltype(auto) get()
	{
		//kompilacji zostanie poddany ten fragment, dla ktorego
		// wyrazenie za if constexpr zwroci true
		if constexpr (N == 0)
			return get_str();
		else if constexpr (N == 1)
			return dbl_;
		else if constexpr (N == 2)
			return int_;
		else if constexpr (N == 3)
			return uint_;
		else if constexpr (N == 4)
			return *ptr_;
	}
};

template<>
struct std::tuple_size<set>
{
	static const size_t value = 5;
};

template<size_t N>
struct std::tuple_element<N, set>
{
	using type = decltype(std::declval<set>().get<N>());
};

//problem lubianych i nie lubianych typow:

template<typename T>
std::false_type type_to_compare;//domyslnie sie nie skompiluje

//dodawanie lubianych typow
template<>
std::true_type type_to_compare<std::string>;

template<>
std::true_type type_to_compare<const char*>;

template<typename T>
bool is_good(T)
{
	//tylko jeden blok zostanie poddany kompilacji
	if constexpr(type_to_compare<T>)
	{
		return true;
	}
	else if constexpr (type_to_compare<T>)
	{
		return false;
	}
}
