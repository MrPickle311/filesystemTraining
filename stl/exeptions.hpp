#pragma once

#include <exception>
#include <system_error>
#include <future>
#include <iostream>

//bad_cast -> zgłasza go operator dynamic_cast ,gdy konwersja na referencji się nie powiedzie
//bad_typeid -> gdy argument typeid jest zerem lub pustym wskaźnikiem
//bad_exception -> służy do wyjątków nieoczekiwanych

//logic_error -> rodzina błędów logicznych oraz naruszających niezmienniki klasy
//invalid_argument -> powiadamia o nieprawidłowych argumentach
//length_error -> przekroczono dopuszczalny rozmiar kolekcji np. dołączono zbyt wiele znaków
//out_of_range -> np. zbyt daleki indeks
//domain_error -> błąd dziedziny
//future_error(zwraca kod) -> błędy logiczne asynchronicznych wywołań systemowych

//runtime_error -> podczas działania programu
//range_error -> np. błąd przy konwersji wstringa na stringa
//overflow_error -> przepełnienie arytmetyczne np. konwersja pola bitowego na wartość całkowitą
//underflow_error -> niedopełnienie arytmetyczne
//system_error (zwraca kod) -> błędy systemu operacyjnego ,często współbieżne błędy
//bad_alloc -> zgłasza go globalny operator new
//bad_array_new_length -> wywołany przez new[] gdy rozmiar jest ujemny lub zbyt duży ,by go można było zaalokować
//bad_weak_ptr -> gdy nie uda się utworzyć słabego wskaźnika ze współdzielonego
//bad_function_call -> gdy std::function będzie wywołane nie mając przypisanego funktora

//ios_base::failure(zwraca kod) -> gdy strumień znajdzie się w nieprawidłowym stanie

//z tej przestrzeni nazw bierzemy wartości wyjątków dla system_error
//std::errc::*

//kody błędów dla przyszłości
//std::future_errc::*

//jedyny kod dostępny dla ios_base::failure to std::io_errc::stream

//kody błędów ogólnie reprezentuje klasa std::error_code
//wartości błędów reprezentuje std::error_condition

template <typename T>
void processCodeException (const T& e)
{
	using namespace std;
	auto c = e.code();
	cerr << "- kategoria: " << c.category().name() << endl;
	cerr << "- wartość: " << c.value() << endl;
	cerr << "- opis: " << c.message() << endl;
	cerr << "- domyśln kategoria: "
	<< c.default_error_condition().category().name() << endl;
	cerr << "- domyślna wartość: "
	<< c.default_error_condition().value() << endl;
	cerr << "- domyślny opis: "
	<< c.default_error_condition().message() << endl;
}

void processException()
{
	using namespace std;
	try
	{
		throw; // ponowne zgłoszenie wyjątku do dopasowania
	}
	catch (const ios_base::failure& e)
	{
		cerr << "WYJĄTEK I/O: " << e.what() << endl;
		processCodeException(e);
	}
	catch (const system_error& e)
	{
		cerr << "WYJĄTEK SYSTEMOWY: " << e.what() << endl;
		processCodeException(e);
	}
	catch (const future_error& e)
	{
		cerr << "WYJĄTEK WYKONANIA ASYNCHRONICZNEGO: " << e.what() << endl;
		processCodeException(e);
	}
	catch (const bad_alloc& e)
	{
		cerr << "WYJĄTEK ALOKACJI: " << e.what() << endl;
	}
	catch (const exception& e)
	{
		cerr << "WYJĄTEK: " << e.what() << endl;
	}
	catch (...)
	{
		cerr << "WYJĄTEK (nieznany)" << endl;
	}
}


void error_testing()
{
	std::error_code c;
	std::error_condition co;
	if(c == co)
		std::cout << "ok\n";
	c.message(); //zwraca komunikat opisu błędu
	bool f = c.operator bool(); //operator bool sprawdza ,czy c chwyciło wyjątek , 0 oznacza brak błędu
	co.category().name(); // składowa name dla error_category zwraca kategorii błędu
	/*W C++ istnieją następujące kategorie błędów:
	 *  "iostream" - I/O
	 *  "generic" - dla system_error gdzie wartość błędu odpowiada wartości zmiennej errno
	 *  "system" - dla system_error gdzie wartość błędu NIE odpowiada wartości zmiennej errno
	 *  "future"
	 *  Każda z kategorii ma swoją globalną funkcję
	 */

	if(co.category() == std::iostream_category())
	{
		//...
	}

	//istnieje klasa std::exception_ptr przechowująca wyjątki

	std::exception_ptr p;
	try
	{
		throw std::system_error{std::make_error_code(std::errc::connection_aborted),"err"};
	}
	catch (...)
	{
		p = std::current_exception();
	}

	//tak mogę przenosić wyjątki do innych miejsc
	if(p)
		std::rethrow_exception(p);
}

