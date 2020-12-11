#pragma once
#include "4.hpp"

template<class CType,class... Args>
void increment(const CType* const ctype,const Args* const... args)
{				//mo�na ��da� wska�nik�w oraz referencji
	CType a = *ctype;
	prnt(++a);
	if constexpr (sizeof...(args) > 0)
		increment(args...);
}

