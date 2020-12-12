#pragma once
#if __has_include("ss.hpp")
	#include "ss.hpp"
#elif __has_include("2.hpp")
	#include "2.hpp"
#endif

#include <iostream>
#include <string>

namespace galaxy
{
	namespace solar_system
	{
		namespace earth
		{
			namespace europe
			{
				namespace poland
				{
					namespace drzewce
					{
						int reader = 2;
					}
				}
			}
		}
	}
}

template<typename Type, template<typename F> typename Container>
class Container_adapter
{
private:
	Container<Type> con_;
public:
	Container_adapter(): con_{}
	{}
	template<typename ... Args>
	void push_back(Args ... vars)
	{
		(con_.push_back(vars), ...);
	}
	void prnt() const
	{
		for (auto x : con_)
			std::cout << x << " ";
		std::cout << "\n";
	}
	decltype(auto) get_container() const
	{
		return con_;
	}
};


/// <summary>
/// ////////////////////
/// </summary>

class super_array
{
	friend class end_itr;
	friend class itr;
private:
	int* array_;
	size_t size_;
public:
	super_array(size_t size) :
		array_{ new int[size] },
		size_{ size }
	{}
	void fill()
	{
		for (size_t i = 0; i < size_; ++i)
			array_[i] = i;
	}
};

class end_itr
{
public:
	end_itr(const super_array& array) :
		arr_{ array.array_ },
		size_{ array.size_ }
	{}
	int* end() const
	{
		return &arr_[size_ - 1] + 1;
	}
private:
	int* arr_;
	size_t size_;
};

class itr
{
	using self = itr&;
public:
	itr(const super_array& array) :
		arr_{ array.array_ },
		pos_{&arr_[0]},
		pos_nmbr_{0}
	{}
	int* begin() const
	{
		return &arr_[0];
	}
	self operator++ ()
	{
		++pos_nmbr_;
		++pos_;
		return (*this);
	}
	int& operator* ()
	{
		return (*pos_);
	}
	int* current_address() const
	{
		return pos_;
	}
	int current_idx() const
	{
		return pos_nmbr_;
	}
private:
	int* arr_;
	int* pos_;
	size_t pos_nmbr_;
};

itr begin(super_array& arr)
{
	return { arr };
}

end_itr end(super_array& arr)
{
	return { arr };
}

bool operator!= (itr m_itr, end_itr e_itr)
{
	return m_itr.current_address() != e_itr.end();
}

