#pragma once

#include <mutex>
#include <condition_variable>
#include <iostream> // na razie
#include <queue>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <future>
#include <thread>
#include <atomic>

/*
//wersja łatwiejsza w implementacji ,ale wolniejsza
//template <typename T>
//class threadsafe_queue
//{
//    ///
//private:
//    mutable std::mutex m_;
//    std::condition_variable data_cond_;
//    std::queue<std::shared_ptr<T>> data_; //tutaj lepiej stosować wskaźniki by nic nie realokować
//                                          // ani danych , ani samego wskaźnika
//    //należy zauważyć ,że w przypadku wystąpienia wyjątku inne wątki nie zostałyby wybudzone
//    //więc należy uniemożliwić wystąpienie wyjątku
//    //kopiowanie egzemplarza std::shared_ptr jest noexcept
//public:
//    threadsafe_queue() {}

//    void push(T nval)
//    {
//        std::shared_ptr<T> val {std::make_shared(std::move(nval))};
//        //wyprowadziłem etap alokacji pamięci w celu podwyższenia
//        //wydajności
//        std::lock_guard g{m_};
//        data_.push(val);
//        data_cond_.notify_one();
//    }

//    void wait_and_pop(T& val)
//    {
//        std::unique_lock l{m_};
//        data_cond_.wait(l,[this]{return !data_.empty();});
//        val = std::move(*data_.front());
//        data_.pop();
//    }

//    std::shared_ptr<T> wait_and_pop()
//    {
//        std::unique_lock l{m_};
//        data_cond_.wait(l,[&]{return !data_.empty();});
//        auto res {data_.front()};
//        data_.pop();
//        return res;
//    }

//    bool try_pop(T& val)
//    {
//        std::unique_lock l{m_};
//        if(data_.empty())
//            return false;
//        val = std::move(*data_.front());
//        data_.pop();
//        return true;
//    }

//    std::shared_ptr<T> try_pop()
//    {
//        std::unique_lock l{m_};
//        if(data_.empty())
//            return nullptr;
//        auto res {data_.front()};
//        data_.pop();
//        return res;
//    }

//    bool empty()
//    {
//        std::lock_guard g{m_};
//        return data_.empty();
//    }
//};
*/

//jednak lepiej to zrobić od zera dla podwyższenia wydajności

template <typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data_;
        std::unique_ptr<node> next_;
    };

	std::unique_ptr<node> head_;
    node* tail_;


	mutable std::mutex head_mutex_;
	mutable std::mutex tail_mutex_;

	std::condition_variable data_cond_;
private:
	node* get_tail()//to trzeba zmienic,widze wyscig do danych
	{
		std::lock_guard g{tail_mutex_};
		return tail_;
	}
	//ta sama kolejnosc blokad , wiec ok
	std::unique_ptr<node> pop_head()
	{
		std::unique_ptr<node> old_head {std::move(head_)};
		head_ = std::move(old_head->next_);
		return old_head;
	}

	std::unique_lock<std::mutex> wait_for_data()
	{
		std::unique_lock head_lock{head_mutex_};
		data_cond_.wait(head_lock,[&] { return  head_.get() != get_tail();});
		return head_lock;
	}

	std::unique_ptr<node> wait_pop_head()
	{
		std::unique_lock l{wait_for_data()};
		return pop_head();
	}

	std::unique_ptr<node> wait_pop_head(T& value)
	{
		std::unique_lock l{wait_for_data()};
		value = std::move(*head_->data_);
		return pop_head();
	}

	std::unique_ptr<node> try_pop_head()
	{
		std::lock_guard g {head_mutex_};
		if (head_.get() == get_tail()) return nullptr;
		return pop_head();
	}

	std::unique_ptr<node> try_pop_head(T& value)
	{
		std::lock_guard g {head_mutex_};
		if (head_.get() == get_tail()) return nullptr;
		value = std::move(*head_->data_);
		return pop_head();
	}
public:
	threadsafe_queue() :
        head_{new node{}},
        tail_{head_.get()}
    {}
	threadsafe_queue(const threadsafe_queue& ) = delete;
	threadsafe_queue& operator= (const threadsafe_queue&) = delete;

    //  sztuczny_wezel <- tail <- i-ty element <- head
    //  do sztucznego węzła nie mam wskaźnika ,ale do taila mam
    //  tail zawsze wskazuje sztuczny węzeł
    void push(T nval)
    {
        //alokacja nowego węzła z danymi
        std::shared_ptr<T>    new_value {std::make_shared<T>(std::move(nval))};
		std::unique_ptr<node> p         {new node}; // to jest ten wirtualny wezel
		{
			node* const	new_tail  {p.get()};// to nie jest rozsądna operacja,ale innego wyjscia nie widze
			std::lock_guard tail_lock{tail_mutex_};
			//zmiana wiązań
			tail_->data_ = new_value;
			tail_->next_ = std::move(p);
			tail_ = new_tail;
		}
		data_cond_.notify_one();
    }

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_ptr<node> const old_head {wait_pop_head()};
		return  old_head->data;
	}

	void wait_and_pop(T& value)
	{
		std::unique_ptr<node> const old_head {wait_pop_head(value)};
	}

    std::shared_ptr<T> try_pop()
    {
		std::unique_ptr<node> old_head {try_pop_head()};                    //nie ma możliwości kopiowania
		return old_head ? old_head->data_ : nullptr;
	}

	bool try_pop(T& value)
	{
		std::unique_ptr<node> const old_head {try_pop_head(value)};
		return old_head != nullptr;
	}

	bool empty()
	{
		std::lock_guard g{head_mutex_};
		return  head_.get() == get_tail();
	}
};


void pop_all(threadsafe_queue<int>& queue,std::atomic_bool& flag)
{
	int val {-1};
	while (true)
	{
		queue.wait_and_pop(val);
		std::cout << val << '\n';
		if (queue.empty() && flag == true)
			break;
	}
}

void push_all(threadsafe_queue<int>& queue,std::atomic_bool& flag)
{
	std::filesystem::path p{"sandbox1/data.txt"};
	std::fstream file{p};
	if(file.is_open())
		std::cout << "Open!" << std::endl;
	int buffer {0};
	while (!file.eof())
	{
		std::cout << "reading!" << std::endl;
		file >> buffer;
		queue.push(buffer);
	}
	flag.store(true);
}

void queue_main()
{
	threadsafe_queue<int> a;
	std::atomic_bool flag{false};
	std::thread th1{push_all,std::ref(a),std::ref(flag)};
	std::thread th2{pop_all,std::ref(a),std::ref(flag)};

	th1.join();
	th2.join();
	std::cout << "Done!" << std::endl;
}
