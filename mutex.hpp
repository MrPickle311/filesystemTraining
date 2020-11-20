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

using namespace std;

string get_text(string text,size_t s)
{
	cout << "computing..." << endl;
	this_thread::sleep_for(chrono::seconds(s));
	return text;
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

	//f1.wait();
	//czyli jak mam async ,to std::async() od razu oblicza i dzieki czemu przy wywolaniu funkcji get() od razu mam wynik
	//gdy mam deffered ,to funkcja std::async() wywoluje zadana funkcje dopiero przy napotkaniu  metody get() lub wait()
	//	cout << f1.get() << endl;
	//	cout << f2.get() << endl;
	//	cout	 << f3.get() << endl;
	//	cout	 << f4.get() << endl;
	//	cout	 << f5.get() << endl;
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

void main_m()
{
	future_tests();
}

