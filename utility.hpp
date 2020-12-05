#pragma once

#include <any>
#include <optional>
#include <variant>
#include <iostream>
#include <string>

std::optional<int> read_int()
{
	int i;
	std::cout << "input:\n";
	if (std::cin >> i)
		return i;
	return {};//niepowodzenie lub brak wartosci
}

std::optional<int> operator+ (std::optional<int> a,std::optional<int> b)
{
	if (!a || !b)//konwersja std::optional<> na bool
		return {};//jesli ,a lub b nie zawiera wartosci ,to zostanie zwrocony brak wartosci
	return {*a + *b};//wyluskanie wartosci z std::optional<int> za pomoca operatora *
}

std::optional<int> operator+ (std::optional<int> a,int b)
{
	if (!a)//konwersja std::optional<> na bool
		return {};//jesli ,a lub b nie zawiera wartosci ,to zostanie zwrocony brak wartosci
	return {*a + b};//wyluskanie wartosci z std::optional<int> za pomoca operatora *
}

struct A1
{
	int a_;
	std::string b_;
};

std::optional<A1> read_struct()
{
	if(int a ; std::cin >> a)
		if (std::string b; std::cin >> b)
			return A1{a,b};
	return {};
}

void optional_tests()
{
	std::cout << "OPTIONAL TESTS" << std::endl;
	auto a {read_int()};
	auto b {read_int()};
	auto sum{a + b + 10};
	//uzycie tych operatorow
	if(sum)//jesli sum nie jest puste
		std::cout << "SUM = " << *sum << std::endl;
	else std::cout << "SUM jest puste! " << std::endl;

	std::optional<A1> a1{read_struct()};
	if(a1)
	{
		std::cout << "A1 jest nie puste i jego wartosci to:\n!" << std::endl;
		std::cout << a1->a_ << "\n" << a1->b_;
	}
	else std::cout << "A1  jest puste!" << std::endl;
}

void util_main()
{
	optional_tests();
}
