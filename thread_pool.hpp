#pragma once
#include "queue.hpp"
#include <numeric>
#include <list>
#include <algorithm>

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

class work_stealing_queue
{
private:
	using data_type = function_wrapper;
	std::deque<data_type> tasks_;
	mutable std::mutex m_;
public:
	work_stealing_queue()
	{}

	work_stealing_queue(const work_stealing_queue&) = delete;
	work_stealing_queue& operator= (const work_stealing_queue&) = delete;

	void push(data_type data)
	{
		std::lock_guard g{m_};
		tasks_.push_front(std::move(data));
	}

	bool empty() const
	{
		std::lock_guard g{m_};
		return tasks_.empty();
	}

	bool try_pop(data_type& res)
	{
		std::lock_guard g{m_};
		if(tasks_.empty())
			return false;

		res = std::move(tasks_.front());
		tasks_.pop_front();
		return true;
	}

	bool try_steal(data_type& res)
	{
		std::lock_guard g{m_};
		if(tasks_.empty())
			return false;
		res = std::move(tasks_.back());
		tasks_.pop_back();
		return true;
	}
};

class thread_pool
{
	//taka kolejka minimalizuje problem wspolzawodnictwa, czyli tego ,ze wszystkie watki gonia
	//by wykonac jedno zadanie ,co marnuje zasoby procesora
	using local_queue_type = std::queue<function_wrapper>;
	using task_type = function_wrapper;
private:
	std::atomic_bool done_;
	threadsafe_queue<task_type> global_work_queue_;
	std::vector<std::unique_ptr<work_stealing_queue>> queues_;
	std::vector<std::thread> threads_;
	thread_joiner joiner_;
//niszczenie obiektow jest prowadzone w odwrotnej kolejnosci niz
//ich deklaracja

	inline static thread_local work_stealing_queue* local_work_queue_;
						//zastosowanie unikalnego wskaznika sprawia ,ze tylko lokalny watek ma dostep
						//do lokalnej kolejki, tutaj juz nie trzeba wspolbieznej kolejki uzywac
	inline static thread_local unsigned my_index_;
private:
	void worker_thread(unsigned my_index)
	{
		my_index_ = my_index;
		local_work_queue_ = queues_[my_index].get();//inicjalizacja statycznej kolejki
		//przypisanie do lokalnej zmiennej jednej z kolejek
		while (!done_)							    //w momencie zakonczenia watku wykonywane sa destruktory pol z modyfikatorami thread_local
			run_pending_task();
	}

	bool pop_task_from_local_queue(task_type& task)
	{
		return local_work_queue_ && local_work_queue_->try_pop(task);
	}

	bool pop_task_from_global_queue(task_type& task)
	{
		return global_work_queue_.try_pop(task);
	}

	bool pop_task_from_other_thread_queue(task_type& task)
	{
		for(unsigned i{0}; i < queues_.size() - 1; ++i)
		{
			unsigned const index = (my_index_ + 1) % queues_.size();	//ten zabieg stosuje sie ,by watki nie probowaly wykrasc
			//zadania tylko z jednej kolejki , tylko z tej ktorej indeks jest nastepny wzgledem lokalnej kolejki watku
			//zapewnia to rownomierne wykradanie zadan
			if(queues_[index]->try_steal(task))
				return true;
		}
		return false;
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
				queues_.push_back(std::unique_ptr<work_stealing_queue> {new work_stealing_queue});
			//konstruktor inicjalizuje lokalne kolejki watkow
			for(size_t i{0}; i < thread_count ;++i)
				threads_.push_back(std::thread{&thread_pool::worker_thread,this,i});
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

		if(local_work_queue_)
			local_work_queue_->push(std::move(task));
		else global_work_queue_.push(std::move(task));

		return res;
	}

	void run_pending_task() // wymuszenie wykonania zadania
	{
		task_type task;
		if (pop_task_from_local_queue(task) ||	//probuje pobrac ze swojej kolejki
				pop_task_from_global_queue(task) ||	//probuje pobrac z globalnej kolejki
				pop_task_from_other_thread_queue(task))	//probuje wykrasc zadanie innemu watkowi
			task();
		else std::this_thread::yield();// zrzekanie sie zasobow procesora
									   //jak nie ma teraz zadan , to chwile poczekaj i
									   //daj szanse innemu watkowi umiescic zadanie
	}
};


//sprawdzeneie tej puli watkow

template<typename Iterator,typename T>
struct accumulate_block
{
	T operator() (Iterator first,Iterator last)
	{
		return std::accumulate(first,last,T{});
	}
};

template<typename Iterator,typename T>
T parallel_accumulate(const Iterator& first,const Iterator& last,T init_value)
{
	std::ptrdiff_t const length {std::distance(first,last)};

	if(length == 0)
		return init_value;

	std::ptrdiff_t  const block_size {25};

	std::ptrdiff_t  num_blocks { (length - 1 ) / block_size};

	if (num_blocks == 0)
		num_blocks = 1;

	std::vector<std::future<T>> futures{num_blocks - 1};

	thread_pool pool;

	Iterator block_start {first};

	for(std::ptrdiff_t  i{0}; i < num_blocks - 1; ++i)
	{
		Iterator block_end {block_start};
		std::advance(block_end,block_size);
		futures[i] = pool.submit([=](){
			return accumulate_block<Iterator,T>{}(block_start,block_end);
		});
		block_start = block_end;
	}

	T last_result {accumulate_block<Iterator,T>()(block_start,last)};
	T result {init_value};

	for(std::ptrdiff_t  i{0}; i < (num_blocks - 1 ); ++i)
		result += futures[i].get();
	result += last_result;
	return result;
}


