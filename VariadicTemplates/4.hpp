#pragma once
#include "3.hpp"
#include <algorithm>

template<class T>
void prnt(T arg)
{
	std::cout << arg;
}

template<size_t Count,char... args>
class Weaver
{
	static_assert((sizeof...(args) != 0), "Brak argumentow!");
	std::vector<char> vec = { args... };
public:
	Weaver()
	{}
	void prnt_this()
	{
		for (size_t i = 0; i < Count; ++i)
		{
			for (auto x : vec)
				prnt(x);
			std::vector<char>::reverse_iterator r_itr_beg = vec.rbegin();
			std::vector<char>::reverse_iterator r_itr_end = vec.rend();
			for (; r_itr_beg != r_itr_end; ++r_itr_beg)
				prnt(*r_itr_beg);
			std::rotate(vec.rbegin(), vec.rbegin() + 1, vec.rend());
			prnt("\n");
		}		
	}
};
