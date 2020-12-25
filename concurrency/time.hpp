#pragma once

#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>

using namespace std;
using namespace chrono;

void duration_tests()
{
	cout << "\nDURATION TESTS" << endl;
	microseconds clock;
//	clock = 3s;
//	while (clock != 0s)
//	{
//		this_thread::sleep_for(clock);
//		cout << clock.count() << endl;
//		clock -= 1s;
//	}

	cout << "dong" << endl;
	auto p1 = high_resolution_clock::now();
	this_thread::sleep_for(5s);
	auto p2 = high_resolution_clock::now();
	cout << duration<long double,ratio<1,1000>>{p2-p1}.count() << endl;
}

void chrono_main()
{
	duration_tests();
}
