#pragma once

#include <iostream>
#include <map>
#include <string>

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

bool pred()
{
	return true or false;
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
