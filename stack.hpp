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

	//takie zaimplementowanie funkcji pop(), czyli bezposrednie zwracanie danych zdjetych ze stosu
	//w odpowiedzi na wywolanie funkcji pop() pozwala wyeliminowac sytuacje wyscigu , ktora zainstanialaby w zwiazku z
	// rozdzieleniem funkcji pop() i top()
	std::shared_ptr<T> pop()
	{
		std::lock_guard g{m_};
		if(data_.empty()) throw empty_stack{};
        auto const value {std::make_shared(std::move(data_.top()))};    //biblioteka standardowa gwarantuje mi to ,ze posprzata po sobie
        //1
        data_.pop();                                                    // w razie wyjatkow ,dzieki temu brak memory leak
		return value;
	}
    ///parę słów o zakleszczeniach
    ///  w miejsciu //1 nastepuje wywolanie zewnetrznej funkcji make_shared
    /// ktora niesie z sobą zagrożenie związane z zakleszczeniem
    /// jednak eliminujemy to stosując lock_guard
    /// zagrożenie jest również w konstruktorach i destruktorze,
    /// ale przecież obiekt można utworzyć i zniszczyć tylko raz ,więc ok

    /// ale z tego co widzę , pojedynczy stos nie jest najlepszy
    /// dla zastosowań współbieżnych

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
