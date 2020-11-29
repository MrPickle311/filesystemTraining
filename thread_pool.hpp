#pragma once
#include "queue.hpp"

class thread_joiner
{
private:
	std::vector<std::thread>& threads_;
public:
	explicit thread_joiner(std::vector<std::thread>&  threads):threads_{threads}
	{}
	~thread_joiner()
	{
		for(auto& thread: threads_)
			if(thread.joinable())
				thread.join();
	}
};

class function_wrapper
{
	struct impl_base
	{
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

private:
	std::unique_ptr<impl_base> impl_;

	template<typename PackagedTaskType>
	struct impl_type :
			public impl_base
	{
		PackagedTaskType f_;
		impl_type(PackagedTaskType&& f): f_{std::move(f)}
		{}
		void call ()
		{
			f_();
		}
	};
public:
	function_wrapper() = default;

	template<typename F>
	function_wrapper(F&& f):
		impl_{new impl_type<F>{std::move(f)}}
	{}

	function_wrapper(function_wrapper&& other):
		impl_{std::move(other.impl_)}
	{}

	void operator() ()
	{
		impl_->call();
	}

	function_wrapper& operator= (function_wrapper&& other)
	{
		this->impl_ = std::move(other.impl_);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete ;
	function_wrapper& operator& (const function_wrapper&) = delete ;
};

class thread_pool
{
private:
	std::atomic_bool done_;
	threadsafe_queue<function_wrapper> work_queue_;
	std::vector<std::thread> threads_;
	thread_joiner joiner_;
//niszczenie obiektow jest prowadzone w odwrotnej kolejnosci niz
//ich deklaracja
private:
	void worker_thread()
	{
		while (!done_)
		{
			function_wrapper task;
			if(work_queue_.try_pop(task))
				task();
			else std::this_thread::yield();//jak nie ma teraz zadan , to chwile poczekaj i
			//daj szanse innemu watkowi umiescic zadanie
		}
	}
public:
	thread_pool():
		done_{false},
		joiner_{threads_}
	{
		size_t const thread_count {std::thread::hardware_concurrency()};

		try
		{
			for(size_t i{0}; i < thread_count ;++i)
				threads_.push_back(std::thread{&thread_pool::worker_thread,this});
		}
		catch (...)
		{
			done_ = true;
			throw;
		}
	}
	~thread_pool()
	{
		done_ = true;
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type; //typ rezultatu tej funkcji ,fajne

		std::packaged_task<result_type()> task{std::move(f)};

		std::future<result_type> res{task.get_future()};

		work_queue_.push(std::move(task));

		return res;
	}
};
