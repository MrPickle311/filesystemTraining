#pragma once

#include <mutex>
#include <condition_variable>
#include <iostream> // na razie
#include <queue>
#include  <condition_variable>

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


//jednak lepiej to zrobić od zera dla podwyższenia wydajności

template <typename T>
class queue
{
private:
    struct node
    {
        std::shared_ptr<T> data_;
        std::unique_ptr<node> next_;
    };

    node* tail_;
    std::unique_ptr<node> head_;
public:
    queue() :
        head_{new node{}},
        tail_{head_.get()}
      //       tail <- virtual_node <- head
    {}
    queue(const queue& ) = delete;
    queue& operator= (const queue&) = delete;

    //  sztuczny_wezel <- tail <- i-ty element <- head
    //  do sztucznego węzła nie mam wskaźnika ,ale do taila mam
    //  tail zawsze wskazuje sztuczny węzeł
    void push(T nval)
    {
        //alokacja nowego węzła z danymi
        std::shared_ptr<T>    new_value {std::make_shared<T>(std::move(nval))};
        std::unique_ptr<node> p         {new node};
        tail_->data_ = new_value;

        //zmiana wiązań
        node* const new_tail = p.get();// to nie jest rozsądna operacja
        tail_->next_ = std::move(p);
        tail_ = new_tail;
    }

    std::shared_ptr<T> try_pop()
    {
        if(head_.get() == tail_)
            return nullptr;
        std::shared_ptr<T>    const res  {head_->data_};
        std::unique_ptr<node> old_head   {std::move(head_)};
        head_ = std::move(old_head->next_); //tu sie uzywa std::move ,gdyż std::unique_ptr
        return res;                      //nie ma możliwości kopiowania
    }
};
