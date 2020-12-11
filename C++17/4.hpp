#pragma once
#include <iostream>
#include <string>

//#define __PRETTY_FUNCTION__ __FUNCSIG__

class Type1
{
public:
	void analysis(Type1* address)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Type2
{
public:
	void analysis(Type2* address)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

template<typename ... Args>
class K : public Args...
{
public:
	using Args::analysis...;
};


class mamal
{
public:
	int heart = 0;
};

class horse : public mamal
{
public:
	std::string legs[4] = { "","","","" };
};
