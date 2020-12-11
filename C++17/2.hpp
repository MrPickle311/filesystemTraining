#pragma once

#include <map>
#include <string>
#include <iostream>
#include <tuple>

struct Data
{
	std::map<std::string, double> data;
	Data(std::initializer_list<std::pair<std::string, double>>&& list):
		data{ list.begin(), list.end() }
	{}
	void show_data() const
	{
		for (auto x : data)
			std::cout << x.first << " " << x.second << "%\n";
	}
	bool provides(const std::string& what)
	{
		return data.find(what) != data.end();
	}
};

struct char_getter
{
	char char_;
	char_getter()
	{
		char_ = getchar();
	}
	char operator () () const
	{
		return char_;
	}
};

void menu()
{
	switch (char_getter g{};g())
	{
	case 'P':
		std::cout << "PLAY" << std::endl;
		break;
	case 'S':
		std::cout << "STOP" << std::endl;
		break;
	case 'F':
		std::cout << "FORWARD" << std::endl;
		break;
	case 'B':
		std::cout << "BACK" << std::endl;
		break;
	case 'X':
		std::cout << "EXIT" << std::endl;
		break;
	default:
		std::cout << "BAD CHAR" << std::endl;
		break;
	}
}

class K
{
public:
	int nmbr_;
	std::string string_;
	double sth_;
	K(int var1, std::string var2, double var3) :
		nmbr_{ var1 },
		string_{ var2 },
		sth_{ var3 }
	{}
};

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

