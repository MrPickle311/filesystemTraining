#pragma once
#include <vector>

template<class VType,class CType,class... Args>
void split_it(std::vector<VType>& small,
			  std::vector<VType>& med,
			  std::vector<VType>& large,
			  CType ctype_val,
			  Args... args)
{	
	if (ctype_val < 10)
		small.push_back(ctype_val);
	else if (ctype_val < 100)
		med.push_back(ctype_val);
	else large.push_back(ctype_val);
	if constexpr(sizeof...(args) > 0)
		split_it(small, med, large, args...);
	return;
}
