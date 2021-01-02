#pragma once

#include <iomanip>
#include <string>
#include <iostream>
#include <limits>

//własne manipulatory
template<typename charT,typename traits>
inline std::basic_ostream<charT,traits>& addColon(std::basic_ostream<charT,traits>& stream)
{
	stream << " : ";
	return  stream;
}

template<typename charT,typename traits>
inline std::basic_istream<charT,traits>& ignore6chars(std::basic_istream<charT,traits>& stream)
{
	stream.ignore(6);
	return  stream;
}

//własny manipulator z argumentem
class ignoreLine
{
private:
	size_t num_;
public:
	explicit ignoreLine(size_t n = 1):
		num_{n}
	{}
	template<typename charT,typename traits>
	friend std::basic_istream<charT,traits>&  operator>>(std::basic_istream<charT,traits>& stream,const ignoreLine& ign)
	{
		for(size_t i{0}; i < ign.num_;++i)
			stream.ignore(std::numeric_limits<std::streamsize>::max(),stream.widen('\n'));
		return stream;
	}
};

//863 niby są własne funkcje formatujące ,ale chyba sobie poradzę
void manip_test()
{	//wszystkie standardowe manipulatory są w ksiażce
	std::string str{"XD"};
	std::cout  << str << addColon << '\n';
	str.clear();
	//std::cin >> ignoreLine{2} >> str;
	std::cout << str << '\n';
}

void format_test()
{
	//szerokość pól jednak nigdy nie może obciąć danych!!!
	//trzeba to samemu zrobić
	//reszta jest w książce ,nieprzydatne jakoś
	std::string str{"dasdada23232"};
	std::cout << "The present width is: " << std::cout.width() << std::endl;
	std::cout.width(20);
	std::cout << "The present width is: " << std::cout.width() << std::endl;
	std::cout.setf( std::ios::left);
	std::cout << std::setw(20) <<str;

	//std::cout << std::endl;
}

void mf_main()
{
	manip_test();
	format_test();
}
