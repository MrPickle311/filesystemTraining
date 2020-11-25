#pragma once

#include <mutex>
#include <iostream>
#include <exception>
#include <memory>
#include <stack>
#include <algorithm>


struct empty_stack : std::exception
{
	const char* what() const noexcept
	{
		return "Empty stack!";
	}
};

template<typename T>
class threadsafe_stack
{
private:
	mutable std::mutex m_;	//kazda funkcja blokuje na poczatku muteks po to by tylko jeden watek naruszal niezmienniki struktury
	// odblokowanie muteksu nie moze zakonczyc sie nie niepowodzeniem
	std::stack<T> data_;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other) :
		m_{}
	{
		std::lock_guard g{other.m_};
		data_ = other.data_;
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;


	void push(T new_value)
	{
		std::lock_guard g{m_};
		data_.push(new_value);
	}

	/// nie wiem ,czy ta metoda jest bezpieczna
	T top () const
	{
		std::lock_guard g{m_};
		if(data_.empty()) throw empty_stack{};
		return data_.top();
	}
	///

	//takie zaimplementowanie funkcji pop(), czyli bezposrednie zwracanie danych zdjetych ze stosu
	//w odpowiedzi na wywolanie funkcji pop() pozwala wyeliminowac sytuacje wyscigu , ktora zainstanialaby w zwiazku z
	// rozdzieleniem funkcji pop() i top()
	std::shared_ptr<T> pop()
	{
		std::lock_guard g{m_};
		if(data_.empty()) throw empty_stack{};
		auto const value {std::make_shared(std::move(data_.top()))};
		data_.pop();
		return value;
	}

	void pop(T& value)
	{
		std::lock_guard g{m_};
		if(data_.empty()) throw empty_stack{};
		value = data_.top();
		data_.pop();
	}

	bool empty() const
	{
		std::lock_guard g{m_};
		return data_.empty();
	}
};
