#pragma once

#include <vector>
#include <iostream>

template<class T>
void prntln(T arg)
{
	std::cout << arg << std::endl;
}

template<int... args>
class Double_class
{
private:
	//tworzenie statycznej tablicy o rozmiarze równym args + 1
	//w tym przypadku musi byæ args + 1 ,gdy¿ iteruje siê od 0
	int tab[sizeof...(args) + 1] = { args... };
	//zosta³a ona zainicjalizowana wartoœciami 0 1 2 3 ....
public:
	Double_class()
	{
		prntln(sizeof...(args) + 1);
	}
	void prnt_this() const
	{
		for (auto x : tab)
			prntln(x);
	}
};
