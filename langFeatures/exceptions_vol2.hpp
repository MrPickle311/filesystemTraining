#pragma once

#include <memory>

//tworzenie pomocniczej klasy do reprezentacji pamięci

//nie jest to kompletny przykład ,lecz tylko pewna ideas
template<typename T,class A = std::allocator<T>>
struct vector_base
{
	T* elem_;  //początek alokacji
	T* space_; //koniec sekwencji elementów,początek przestrzeni zaalokowanej do ewentualnego rozszerzania
	T* last_;  //koniec alokowanej pamięci
	A  alloc_;  // alokator

	vector_base(const A& a,typename A::size_type n):
		alloc_{a},
		elem_{alloc_.allocate(n)},
		space_{elem_ + n},
		last_{space_}
	{}
	virtual ~vector_base()
	{
			alloc_.deallocate(elem_,last_ - elem_);
	}
	vector_base(const vector_base&) = delete;
	vector_base& operator= (const vector_base&) = delete;

	vector_base(vector_base&& a):
		elem_{a.elem_},
		space_{a.space_},
		last_{a.last_},
		alloc_{a.alloc_}
	{
		a.elem_ = a.space_ = a.last_ = nullptr; // pamięć została przeniesiona
	}
	vector_base& operator= (vector_base&& a)
	{
		std::swap(*this,a);
		return *this;
	}
};

template<typename T,typename A = std::allocator<T>>
class vector
{
private:
	vector_base<T,A> vb_;
	void destroy_elements()
	{
		for(T* p = vb_.elem_; p != vb_.space_;++p)
			p->~T();
		vb_.space_  = vb_.elem_;
	}
public:
	using size_type = unsigned int;
	using iterator = T*;

	explicit vector(size_type n,const T& val = T{}, const A&  a = A{}):
		vb_{a,n}
	{
		std::uninitialized_fill(vb_.elem_,vb_.elem_ + n,val);
	}

	T* begin() const;
	T* end() const;
	std::allocator<T> alloc() const;

	vector(const vector& a):
		vb_{a.alloc(),a.size()}
	{
		std::uninitialized_copy(a.begin(),a.end(),{});
	}
	vector& operator= (const vector& a);

	vector(vector&& a);
	vector& operator= (vector&& a);

	~vector()
	{
		destroy_elements();
	}

	size_type size() const
	{
		return vb_.space_ - vb_.elem_;
	}

	size_type capacity() const
	{
		return vb_.last_ - vb_.elem_;
	}

	void reserve(size_type n);

	void resize(size_type n,const T& = {});
	void clear()
	{
		resize(0);
	}
	void push_back(const T& x) // w razie nieudanego dodania elementu , kontener powinien pozostać bez zmian
	{
		//if(capacity() == size()) // potrzebna realokacja
		//	reserve(size())
	}
};
