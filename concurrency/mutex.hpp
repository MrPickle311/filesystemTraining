#pragma once
#include <mutex>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <condition_variable>
#include <string>
#include <chrono>
#include <deque>

using namespace std;

string get_text(string text,size_t s)
{
	cout << "computing..." << endl;
	this_thread::sleep_for(chrono::seconds(s));
	return text;
}

void get_text_with_promise(string text,size_t s,promise<string>& pr)
{
	cout << "computing..." << endl;
	this_thread::sleep_for(chrono::seconds(s));
	pr.set_value(text);
}

int malicious_funtion(int i)
{
	if ( i< 0)
		throw out_of_range{" i < 0"};
	return i;
}

void future_status_check(future_status s,size_t n)
{
	cout << "Function " << n << " invoked" << endl;
	if(s == future_status::ready)
		cout << "Done!" << endl;
	else if (s == future_status::deferred)
		cout << "Deffered!" << endl;
	else cout << "Timeout!" << endl;
}

void future_tests()
{
	cout << "\nFUTURE TESTS" << endl;
	cout << thread::hardware_concurrency() << endl;
	future<string> f1 {async(launch::deferred,get_text,"text1",5)};
	future<string> f2 {async(launch::async,get_text,"text2",5)};
	future<string> f3 {async(launch::deferred,get_text,"text3",5)};
	future<string> f4 {async(launch::deferred,get_text,"text4",5)};
	future<string> f5 {async(launch::deferred,get_text,"text5",5)};

	future_status s = f1.wait_for(chrono::seconds(60)); //tutaj nawet nie czeka na zadana funkcje , bo nie ma na co czekac

	future_status s1 = f2.wait_for(chrono::seconds(1));
	//funkcja wait_for() nie wywoluje zadanej funkcji , ona tylko czeka okreslona ilosc czasu
	//lecz sama funkcje wait() wywoluje funkcje
	//gdy wywolanie jest lanuch::deferred funkcja wait_for() od nawet nie probuje wywolac zadanej funkcji

	f1.wait();
	//czyli jak mam async ,to std::async() od razu oblicza i dzieki czemu przy wywolaniu funkcji get() od razu mam wynik
	//gdy mam deffered ,to funkcja std::async() wywoluje zadana funkcje dopiero przy napotkaniu  metody get() lub wait()
		cout << f1.get() << endl;
		cout << f2.get() << endl;
		cout	 << f3.get() << endl;
		cout	 << f4.get() << endl;
		cout	 << f5.get() << endl;
	//by zobaczyc efekt , oproznij bufor
	future_status_check(s,1);

	try {
		cout << f2.get() << endl;
		//get rowniez czeka , nie zglasza wyjatku ,gdy jeszcze nie ma wyniku ,tylko na niego czeka
	}  catch (future_error& e) {
		cout << e.what() << endl;
	}

	future_status_check(s1,2);
}

void packaged_task_tests()
{
	cout << "\nPACKAGED TASKS TESTS" << endl;

	//wszystko jest wykonywane po kolei
	cout << "\nsync" << endl;
	packaged_task<string(string,size_t)> task {get_text};
	future<string> f1 = task.get_future();
	task("Hello!",5);
	cout << "Waiting..." << endl;
	cout << f1.get() << endl;

	//asynchronicznie
	cout << "\nasync" << endl;
	packaged_task<string(string,size_t)> task2 {get_text};
	if(task2.valid()) // nie moge wykonac ponizszych operacji ,gdy valid() == false
		//valid() patrzy ,czy zostalo przydzielone zadanie
	{
		future<string> f2 = task2.get_future();//wiazanie zadania z std::future
		thread th1{move(task2),"Hello!",5};//oprocz funkcji get brak jakiegokolwiek kontaktu z watkiem th1

		cout << "Waiting" << endl;

		cout << f2.get() << endl;
		th1.join();
	}
	else cout << "Task is not valid" << endl;

	//obsluga wyjatku
	cout << "\nexception test" << endl;
	packaged_task<int(int)> task3 {malicious_funtion};
	future<int> f3 = task3.get_future();
	try {
		task3(-1); //tutaj jeszcze nie nastepuje zgloszenie wyjatku
		cout << f3.get() << endl; // lecz nastepuje ono tutaj,zamiast pobrania wartosci propagowany jest wyjatek
	}  catch (out_of_range& e) {
		cout << "exception has been propagated : " << e.what() << endl;
	}

}

void writing_loop (promise<bool>& flag_promise)
{
	for(size_t i{0}; i < 5 ; ++i)
	{
		cout << "Waiting : " << i << endl;
		if (i == 2) flag_promise.set_value(true);
		this_thread::sleep_for(chrono::seconds(2));
	}
}

void err(promise<int>& p)
{
	try {
		throw runtime_error{"Error!"};
	}  catch (...) {
		p.set_exception(current_exception());
	}
}

void sth_thread_func(promise<void>& pr1,promise<void>& pr2)
{
	future<void> f5{pr2.get_future()};
	this_thread::sleep_for(chrono::seconds(3));
	pr1.set_value();
	cout << "Waiting for signal 2 " << endl;
	f5.wait();
	cout << "Signal 2 received" << endl;
	cout << "Processing" << endl;
	this_thread::sleep_for(chrono::seconds(3));
}

void promise_tests()
{
	cout << "\nPROMISE TESTS" << endl;
	promise<string> string_promise;
	future<string> f1 {string_promise.get_future()}; // wiazanie przyszlosci z std::promise
	thread th{get_text_with_promise,"Hello!",3,ref(string_promise)};// ten watek ma kontakt z odosobnionym watkiem ,dzieki
	//wyslaniu do niego referencji obiektu std::future
	cout << f1.get() << endl;// wartosc lub wyjatek bedzie gotowy do pobrania ,gdy obiekt std::promise
	//wywola funkcje set_value() lub set_exception() w wywolywanej funkcji
	th.join();

	//kontakt miedzy watkiami
	promise<bool> promise_flag;
	future<bool> f2{promise_flag.get_future()};
	thread th2{writing_loop,ref(promise_flag)};
	cout  << boolalpha << f2.get() << endl;
	th2.join();

	//obsluga bledow
	promise<int> pr;
	future<int> f3 {pr.get_future()}; // powiazanie
	thread th3{err,ref(pr)};
	try {
		cout << f3.get() << endl;
	}  catch (exception& e) {
		cout << e.what() << endl;
	}
	th3.join();

	//emulacja przekazywania sygnalow miedzy watkami z wykorzystaniem promise<void>
	//osobny watek th4 wysyla mi jakis sygnal,potem go przetwarzam tutaj i wysylam sygnal o koncu przetwarzania ,po czym nastepuje koniec
	//potem to zrob poprzez czysczenie , o ile mozliwe
	promise<void> pr1;
	promise<void> pr2;
	future<void> f4{pr1.get_future()};

	cout << "Launching external thread..." << endl;

	thread th4{sth_thread_func,ref(pr1),ref(pr2)};

	f4.wait();
	cout << "Signal 1 received" << endl;
	cout << "Processing..." << endl;

	this_thread::sleep_for(chrono::seconds(3));
	pr2.set_value();

	cout << "Waiting... " << endl;

	th4.join();
	cout << "End " << endl;
}

void task(string str,size_t s)
{
	this_thread::sleep_for(chrono::seconds(s));
	cout << "Task " << str << " done! " << endl;
}

class tasks_executer//to wymaga puli watkow
{
public:
	tasks_executer(deque<packaged_task<void(string,size_t)>> functions,deque<pair<string,size_t>> args):
		avalaible_threads_{3},
		tasks_{move(functions)},
		args_{args}
	{}
private:
	mutable size_t avalaible_threads_;
	mutable mutex m_;
	deque<packaged_task<void(string,size_t)>> tasks_;
	deque<pair<string,size_t>> args_;
	condition_variable cv;
private:
	void pop_task()
	{
		//tutaj szybkie pobranie zadania

		packaged_task<void(string,size_t)> task = move(tasks_.front());
		tasks_.pop_front();

		future<void> f {task.get_future()};
		thread th{move(task),args_.front().first,args_.front().second};
		f.get();
		th.join();

		++avalaible_threads_;
		cv.notify_one();
	}
public:
	void execute_tasks()
	{
		while (!tasks_.empty())
		{
			unique_lock g{m_};//trzeba uzyc unique_lock gdyz istnieje potrzeba elastycznego odblokowywania i zablokowywania
			cv.wait(g,[&]{ return avalaible_threads_ > 0 ;});
			--avalaible_threads_;	//to tutaj trzeba sekwencyjnie odejmowac ilosc watkow
			pop_task();
		}
	}
};


void condition_variable_test()
{
	cout << "\nCONDITION VARIABLE TESTS" << endl;
	cout << "Generatings tasks " << endl;
	deque<packaged_task<void(string,size_t)>> tasks_;
	deque<pair<string,size_t>> args_;
	for(size_t i{0}; i < 10;++i)
	{
		tasks_.push_back(packaged_task<void(string,size_t)>{task});
		args_.push_back({"Task",3});
		cout << "Task : " << i << " generated" << endl;
	}
	tasks_executer t1{move(tasks_),move(args_)};
	t1.execute_tasks();
}

void waiting_func(shared_future<void> f,size_t n,mutex& m)
{
	f.wait();
	lock_guard l{m};
	cout << "Function : " << n << " executed" << endl;
}

void shared_future_tests()
{
	cout << "\nSHARED FUTURE TESTS" << endl;
	promise<void> promise;
	shared_future<void> f1{promise.get_future()};
	mutex m;
	thread th1{waiting_func,f1,1,ref(m)};
	thread th2{waiting_func,f1,2,ref(m)};
	//dzieki mozliwosci kopiowania obiektu std::shared_future moge oczekiwac jednego wyniku w dwoch roznych funkcjach
	this_thread::sleep_for(4s);//4 sekundy
	promise.set_value();

	th1.join();
	th2.join();
}

void main_m()
{
	//future_tests();
	//packaged_task_tests();
	//promise_tests();
	//condition_variable_test();
	shared_future_tests();
}

