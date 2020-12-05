#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <numeric>
#include <list>
#include <random>

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

struct pred1
{
	bool operator() (size_t a)
	{
		return  a > 15;
	}
};

void non_modyfing_alghorithms_with_map()
{
	std::cout << "\nNON MODYFING ALGORITHMS!" << std::endl;
	std::map<std::string,size_t> elements;

	for(size_t i{0}; i < 30;++i)
		elements.insert({std::to_string(i),i});


	//size_t c1 {std::count(elements.begin(),elements.end(),2)};
	//count tutaj niespecjalnie zadziala
	//trzeba uzyc count_if

	auto pred = [](std::pair<const std::string,size_t>& node )->bool
	{
		return node.second > 15;
	};

	size_t c2 {static_cast<size_t>(std::count_if(elements.begin(),elements.end(),pred))};
	//diffrence type musisz static_castowac!!!!

	std::cout << c2 << std::endl;

	std::cout << '\n';///

	using map_type = std::map<std::string,size_t>;
	using cmap_type_ref = const std::pair<std::string,size_t>&;

	std::pair<map_type::iterator,map_type::iterator> p {std::minmax_element(elements.begin(),elements.end(),
														 [&](cmap_type_ref map1,cmap_type_ref map2)
															{
																return map1.second < map2.second;
															}
														 )};
	std::cout << p.first->first << " " << p.second->second << std::endl;
	//aglorytmy z tej rodziny zwracaja iteratory , przyjmuja binarny predykat

	std::cout << "\n" << std::endl;///

	map_type::iterator p1{std::find_if(elements.begin(),elements.end(),[&](cmap_type_ref e)
		{
			return e.first == "6";
		})};
	std::cout << p1->second << std::endl;

	std::cout << '\n';

	map_type::iterator itr {elements.begin()};

	std::advance(itr,15);

	for(size_t i{0}; i < 5 ;++i)
		elements.insert(itr,{"5",5});

	std::cout << "Printing map...\n";
	for(size_t i{0}; i < elements.size() ;++i)
		std::cout << elements[std::to_string(i)] << " ";

	std::multimap<std::string,size_t> m_elements;

	for(size_t i{0}; i < 5 ;++i)
		m_elements.insert(itr,{"5",5});
	for(size_t i{0}; i < 5 ;++i)
		m_elements.insert(itr,{"1",1});
	std::cout << "Printing multimap...\n";
	for(auto& element : m_elements)//tutaj brak funkcji at()
		std::cout << element.first << " ";

	std::cout << '\n';

	std::pair<std::string,size_t> const pr{"5",5};
	//std::search_n(m_elements.begin(),m_elements.end(),4,pr);
	//to sie nigdy nie skompiluje ,gdyz mapa nie dopuszcza kilku wezlow o tym samym kluczu
	///ogarnac dlaczego ten chuj sie nie kompiluje

	std::cout << '\n';

	std::multimap<std::string,size_t> m_elements2;

	//mapy i multimapy sa automatycznie sortowane !!!
	//ale i tak ich sie nie przeglada tak jak list ,czy wektorow
	for(size_t i{0}; i < 5 ;++i)
		m_elements2.insert(itr,{"5",5});
	for(size_t i{0}; i < 5 ;++i)
		m_elements2.insert(itr,{"1",1});

	auto help_itr {m_elements2.begin()};
	std::advance(help_itr,6);

	auto itr2 {std::search(m_elements2.begin(),m_elements2.end(),help_itr,m_elements2.end())};
	//widac ,ze aglorytmy standardowe niespecjalnie wspolpracuja z mapami ,ale to dlatego ,ze to drzewa binarne

	std::list<std::pair<std::string,size_t>> m_elements3;
	for(size_t i{0}; i < 5 ;++i)
		m_elements3.push_back({"5",5});
	for(size_t i{0}; i < 5 ;++i)
		m_elements3.push_back({"1",1});

	auto help_itr2 {m_elements2.begin()};
	std::advance(help_itr2,6);

	//auto itr3 {std::search(m_elements3.begin(),m_elements3.end(),help_itr2,m_elements3.end())};
	//mutlimapy to rowniez drzewa binarne ,wiec nic z tego

	std::list<int> collection{1,23,2323,13,213,1,1,1,3,23,23,23,-1,-1};
	std::list<int> sub_collection{1,3,23,23};

	auto pred1 = [&](const int& e,const size_t& val){
		return  e % 2 != 0 && val % 2 != 0;
	};

	auto itr4 {std::search_n(collection.begin(),collection.end(),3,1,pred1)};
	std::cout << *itr4 << " " << *(++itr4)  << std::endl;
	std::cout << '\n';
	//tutaj predykat ma wyzszy priorytet

	auto itr5 {std::search_n(collection.begin(),collection.end(),3,1)};
	std::cout << *itr5<< " " << *(++itr5)  << std::endl;
	std::cout << '\n';
	//zwraca pierwsza pozycje podzasiegu

	std::list<int> sub2{8,86};

	auto itr6 {std::search(collection.begin(),collection.end(),sub_collection.begin(),sub_collection.end())};
	std::cout << *itr6<< " " << *(++itr6)  << std::endl;
	std::cout << '\n';

	auto itr7 {std::search(collection.begin(),collection.end(),sub2.begin(),sub2.end())};
	if(itr7 == collection.end())
		std::cout << "Not found!\n";
	if(itr7 == sub2.end())
		std::cout << "Not found! again! \n";
	//jezeli spodziewamy sie ,ze dana wartosc/podciag nie zostanie znaleziony ,to
	//porownujemy z iteratorem .end() , ktory nalezy do przeszukiwanego podciagu

	std::cout << '\n';

	auto itr8 {std::search(collection.begin(),collection.end(),sub_collection.begin(),sub_collection.end(),pred1)};
	std::cout << *itr8<< " " << *(++itr8)  << std::endl;
	std::cout << '\n';
	//przy kazdym wywolaniu pred musi zwracac wartosc true

	//find_end dziala tak samo ,lecz zwraca pierwszy element ostatniego podzakresu

	std::cout << "\n\n";

	std::vector<int> v1 {2,35,32,1,2,32,1,4,1};
	std::vector<int> v2 {32,1};

	auto itr9 {std::find_first_of(v1.begin(),v1.end(),v2.begin(),v2.end())};

	std::cout << *itr9<< " " << *(++itr9)  << std::endl;
	std::cout << '\n';

	auto pred3 = [=](int elem,int searchElem)
	{
		return elem % 2 == 1 && searchElem % 2 == 0;
	};

	auto itr10 {std::find_first_of(v1.begin(),v1.end(),v2.begin(),v2.end(),pred3)};

	//druga postac zwraca pozycje pierwszego elementu w zakresie beg
	//end dla ktorego wywolanie pred z dowolna wartoscia z serachBeg i searchEnd
	//zwraca true
	std::cout << *itr10<< " " << *(++itr10)  << std::endl;
	std::cout << '\n';


	std::cout << "END!\n" << std::endl;
}

void baa_main()
{
	//bind_testing();
	non_modyfing_alghorithms_with_map();

}
