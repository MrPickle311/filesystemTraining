#pragma once

#include <thread>

class joining_thread
{
	std::thread t_;
public:
	joining_thread() noexcept = default;

	template<typename Callable,typename...Args>
	joining_thread(Callable&& func,Args&&...args): //tutaj używam std::forward ,gdyż nie wiem ,czy dostanę rvalue ,czy lvalue
		t_{
			std::forward<Callable>(func),
			std::forward<Args>(args)...
		   }
	{}

	explicit joining_thread(std::thread t) noexcept:
		t_{std::move(t)}
	{}

	explicit joining_thread(joining_thread&& other) noexcept:
		t_{std::move(other.t_)}
	{}

	joining_thread& operator=(joining_thread&& other)
	{
		if(t_.joinable())
			t_.join();
		t_ = std::move(other.t_);
		return *this;
	}

	joining_thread& operator=(std::thread other)
	{
		if(t_.joinable())
			t_.join();
		t_ = std::move(other);
		return *this;
	}

	~joining_thread() noexcept
	{
		if(t_.joinable())
			t_.join();
	}

	void swap(joining_thread& other) noexcept
	{
		t_.swap(other.t_);
	}

	std::thread::id get_id() const noexcept
	{
		return this->t_.get_id();
	}

	bool joinable() const noexcept
	{
		return this->t_.joinable();
	}

	void join()
	{
		this->t_.join();
	}

	void deteach()
	{
		this->t_.detach();
	}

	std::thread& as_thread() noexcept
	{
		return this->t_;
	}

	const std::thread& as_thread() const noexcept
	{
		return this->t_;
	}
};
