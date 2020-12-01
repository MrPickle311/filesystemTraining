#pragma once

#include <algorithm>
#include <functional>
#include <iostream>

char toUpper(char c)
{
	return std::toupper(c);
}

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

	std::transform(s.begin(),s.end(),s.begin(),to_upper);

	std::cout << s << std::endl;


}

void baa_main()
{
	bind_testing();
}
