#pragma once

//*inline informuje linker, że każdy symbol  o danej sygnaturze i danej definicji może pojawić się w kilku jednostkach kompilacji
//i w takiej sytuacji linker może wybrać dowolną z nich a resztę odrzucić. Należy tutaj też podkreślić jedną rzecz – jeżeli
//w tej samej jednostce kompilacji znajdą się definicje dwóch takich samych funkcji, kompilator ma obowiązek zgłosić błąd.
//*inline sugeruje kompilatorowi, żeby wstawił ciało funkcji w miejsce wywołania, zamiast samego wywołania.

/*np.
 *
//pi.h
#pragma once
float calculate_pi();

pi.cpp
#include "pi.hpp"
#include <iostream>

inline void log(const char* cstring)
{
	std::cout << "[ PI ]: " << cstring << std::endl;
}

float calculate_pi()
{
	log("zaczynam liczyc");
	log("koncze liczyc");
	return 3.1415;
}

//e.h
#pragma once
float calculate_e();

//e.cpp
#include "e.hpp"
#include <iostream>

inline void log(const char* cstring)
{
	std::cout << "[ e ]: " << cstring << std::endl;
}

float calculate_e()
{
	log("zaczynam liczyc");
	log("koncze liczyc");
	return 2.7183;
}

//main.cpp

#include "pi.hpp"
#include "e.hpp"
#include <iostream>

int main()
{
	float pi = calculate_pi();
	float e = calculate_e();
	std::cout << "liczba PI: " << pi << "\nliczba e: " << e << std::endl;
	return 0;
}

*/

//Tutaj będą dwie jednostki translacji ,więc linker wybierze dowolną funkcję log
//a resztę odrzuci

//Niejawne oznaczenie inline pojawia się w dwóch sytuacjach:
//1.przy wszyskim ,co jest constexpr
//2.funkcje zdefiniowane w ciele klasy + friend

//w C++17 można zrobić to ze zmiennymi globalnymi
inline constexpr auto THREAD_POOL_SIZE{5};
inline double globalAppConfig{};


//statyczne pola klasy

//wcześniej

struct y
{
	static int r;
};

int y::r = 2;

//teraz
class x
{
public:
	inline static int g = 0;
};

//drugie zastosowanie ,to sobie ogarnia kompajler
