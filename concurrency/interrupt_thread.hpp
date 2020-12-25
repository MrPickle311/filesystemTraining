#pragma once

#include <thread>
#include <memory>
#include <future>

class interrupt_flag
{
public:
	void set();
	bool is_set() const;
};
thread_local interrupt_flag this_thread_interrupt_flag;

class interruptable_thread
{
private:
	std::thread internal_thread_;
	interrupt_flag* flag_; //!!NAGI WSKAŹNIK!!
public:
	template<typename Func>
	interruptable_thread(Func f)
	{
		std::promise<interrupt_flag*> p;

		internal_thread_ = std::thread{[f,&p]
		{
			p.set_value(&this_thread_interrupt_flag);
			f();
		}};

		flag_ = p.get_future().get();
	}
	void join();
	void deteach();
	bool joinable() const;
	void interrupt()
	{
		if(flag_)
			flag_->set();
	}
};
