#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <numeric>

char toUpper(char c)
{
	return std::toupper(c);
}

class modifier
{
private:
	int a_;
public:
	modifier(int a) : a_{a}
	{}
	int modify(int b)
	{
		a_ = b;
		return a_ + b;
	}
	int operator() (int b)
	{
		return  a_ + b;
	}
};

void bind_testing()
{
	auto plus10 { std::bind(std::plus<int>(),std::placeholders::_1,std::placeholders::_2)};

	std::cout << plus10(7,4) << std::endl;

	auto plus10times2 { std::bind( std::multiplies<int>() ,
								   std::bind(std::plus<int>() ,
								   std::placeholders::_1,10),
								   2 )};

	std::cout << plus10times2(7) << std::endl;

	std::vector<int> v{1,2,3,4,5,6,7,8,9};

	auto negate_and_mult2times{ std::bind(std::negate<int>(),std::bind(std::multiplies<int>(),std::placeholders::_1,2))};
	//wyrazenia sa obliczane od srodka na zewnatrz

	auto negate_and_pow2{ std::bind(std::negate<int>{},std::bind(std::multiplies<int>(),std::placeholders::_1,std::placeholders::_1))};

	std::transform(v.begin(),v.end(),v.begin(),negate_and_mult2times);

	for(auto& x : v)
		std::cout << x << std::endl;

	std::transform(v.begin(),v.end(),v.begin(),negate_and_pow2);
	std::cout << std::endl;
	for(auto& x : v)
		std::cout << x << std::endl;

	std::string s {"i hate comunnism"};
	auto to_upper {std::bind(toUpper,std::placeholders::_1)};

	std::string sub{"hate"};

	auto pred {std::bind(std::equal_to<char>{},std::bind(toUpper,std::placeholders::_1),std::bind(toUpper,std::placeholders::_2))};

	auto pos {std::search(s.begin(),s.end(),sub.begin(),sub.end(),pred)};

	std::cout << *pos << std::endl;

	std::transform(s.begin(),s.end(),s.begin(),to_upper);

	std::cout << s << std::endl;

	std::cout << "\nMethods" << std::endl;

	modifier m{5};

	auto modify {std::bind(&modifier::modify,std::placeholders::_1,std::placeholders::_2)};
	//1-szy argument jest obiektem ,dla ktorego wywoluje metode modify
	std::cout << modify(m,6) << std::endl;

	std::vector<modifier> v1{modifier{5},modifier{0},modifier{-1},modifier{6},modifier{2}};

	//auto executer {}

	int i{0};
	for(auto& obj: v1)
	{
		std::cout << modify(obj,i) << std::endl;
		++i;
	}

	std::shared_ptr<modifier> m1{new modifier{5}};

	std::cout <<"\n" << modify(m1,6) << std::endl;

	//adaptator specjalny dla metod obiektow
	//dzieki niemu nie musze jawnie podawac
	//dla jakiego obiektu podaje metode
	//std::for_each(v1.begin(),v1.end(),std::mem_fn(&modifier::modify));

	//uzyskiwanie dostepu do skladowych

	std::map<std::string,int> map;
	map.emplace("1",1);
	map.emplace("2",2);
	map.emplace("3",3);

	int sum {std::accumulate(map.begin(),map.end(),
							 0,
							 std::bind(std::plus<int>{},
									   std::placeholders::_1,
									   std::bind( &std::map<std::string,int>::value_type::second , std::placeholders::_2) //wiaze
									   //drugi operand z &std::map<std::string,int>::value_type::second
									   )
							 )};

	std::cout << "\n" << sum << std::endl;
}

void baa_main()
{
	bind_testing();
}
