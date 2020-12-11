#pragma once
#include <iostream>

template<class... Args>
void prnt(Args... args)
{
	//std::cout << cval << std::endl;
	//if constexpr (sizeof...(args) > 0)
	//	prnt(args...);
	int array[sizeof...(args) + 1] = { (std::cout << args << " ",0)... };
}

template<class CType, class... Args>
CType sum(CType cval, Args... args) 
{
	CType val = cval;
	if constexpr (sizeof...(args) > 0)
		val += sum(args...);
	return val;
}

void endl()
{
	std::cout << std::endl << std::endl;
}

template<class... Args>
void do_sth( Args... args)
{
	//na paczkach argumentów mo¿na dokonywaæ 
	//zbiorowe operacje arytmetyczne
	prnt(args...);
	endl();
	prnt(2 * args...);
	endl();
	prnt(args + 5 ...);
	endl();
	prnt(args * args...);
	endl();
	prnt(args..., args...);
	endl();
	prnt(sum(args...) + args...);
}
