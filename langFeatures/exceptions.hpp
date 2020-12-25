#pragma once

#include <type_traits>
#include <iostream>
#include <memory>
#include <future>
#include <thread>
#include <vector>
#include <exception>

template<typename T>
void my_func() noexcept(std::is_const<T>()) // jeśli constexpr predykat okaże się prawdziwy, to ta funkcja będzie noexcept
{
	//jeśli skłamiemy w funkcji noexcept ,to zostanie wywołane std::terminate
	//oraz nastąpi wyciek pamięci
}

//W c++17 nie wolno używać specyfikacji wyjątków
//void g() throw(std::runtime_error)
//{

//}

//załóżmy ,że mamy taką funkcję:

struct A{};

struct B: A {};

void g()
{
	//..


	try
	{
		throw B{};
	}
	catch (A& a)
	{
		//...
	}
	//..
}

/*tutaj wyjątek zostanie obsłużony tylko wtedy ,gdy :
 * - typ A to ten sam co B
 * typ A jest bazą B
 * działa również ze wskaźnikami referencjami
 */

//ponawianie zgłoszenia wyjątku
//stosuje się to ,gdy obsługa błędu jest podzielona na kilka procedur

bool can_handle_it_completely();

void h()
{
	try
	{
		//...
	}
	catch (std::exception& e)
	{
		if(can_handle_it_completely())
		{
			//obsługa wyjątku
		}
		else
		{
			//zrobienie co się da
			throw;//tutaj zostanie ponownie wypropagowany ten sam wyjątek
		}
	}
}

//można użyć wielu procedur obsługi wyjątków
//można też przechwytywać każdy rodzaj wyjątku
void m()
{
	try
	{
		//..
	}
	catch(std::runtime_error& e)
	{
		//..
	}
	catch(std::logic_error& e)
	{
		//..
	}
	catch (...) // ta klauzula musi być ostatnia
	{

		//..porządkowanie wskaźników
		throw;
	}
}

//Funkcje jako bloki try
//treść funkcji może być w całości blokiem try

//dobre narzędzie do pracy z inicjatorami

class X
{
	std::vector<int> vi;
	std::vector<std::string> vs;
public:
	X(int sz1,int sz2)
	try
		:vi{sz1},//możliwość przechwytywania wyjątków zgłaszanych przez konstruktory składowych
		vs{std::to_string(sz2)}
	{

	}
	catch (std::exception& err)
	{
		//tutaj nie specjalnie można tak sprzątać
		//jedynie ,co można zrobić to dalej wypropagować wyjątek
		//nie dotyczy to zwykłych funkcji
		vi.~vector();
	}
};


//funkcja std::terminate() zostanie wywołana gdy:
/* 1.nie obsłużymy za pomocą catch zgłoszonego wyjątku
 * 2.funkcja noexcept zgłosi wyjątek
 * 3.gdy destruktor podczas odwijania stosu wywoła throw ( destruktory rób jako noexcept)
 * 4.gdy kod wywołany w celu propagacji wyjątku użyje throw
 * 5.gdy próbuje się zgłosić wyjątek podczas gdy nie jest obsługiwany żaden wyjątek (?)
 */

//destruktory są niejawnie jako funkcje noexcept
//żeby zgłaszasły wyjątki bez std::terminate trzeba dać:
//noexcept(false)
//każda klasa bazowa również musi mieć noexcept(false) przy destruktorze

//nie używaj nagiego std::thread ,jeżeli pomiędzy jego konstruktorem , a destruktorem
//zostaje zgłoszony wyjątek ,wtedy program wywoła std::terminate

///void parallelRun(function<void()> task1, function<void()> task2)
///{
 ///   std::thread th{ task1 };
///    task2();   // throws exception!
 ///   th.join(); // join omitted
///}              // terminate!

//w środowisku wielowątkowym można przekazywać między sobą wątki
void j()
{
	std::promise<int> p;
	try
	{
		//..
	}
	catch (...)
	{
		p.set_exception(std::exception_ptr{});
	}
}

//pamiętaj ,że podczas odwijania stosu ,jeśli wywołasz w jakimś destruktorze
//inną funkcję i ona wywoła jakąś inną funkcję itd.. to wtedy nawijasz stos
//czyli dokładasz tych funkcji

//Implementacja wektora jako przykład

//używaj bloku try oraz RAII

//Są dwie zasady:
//Nigdy nie wypuszczaj informacji ,dopóki nie ma czym jej zastąpić
//Zgłaszając wyjątek zawsze pozostawiaj obiekty w poprawnym stanie

//narzut od gwarancji podstawowej jest bardzo mały,zaniedbywalny

//uproszczona deklaracja wektora

template<typename T,class A = std::allocator<T>>
class vector
{
private:
	T* elem_;  //początek alokacji
	T* space_; //koniec sekwencji elementów,początek przestrzeni zaalokowanej do ewentualnego rozszerzania
	T* last_;  //koniec alokowanej pamięci
	A  alloc_;  // alokator
public:
	using size_type = unsigned int;
	using iterator = T*;
	//naiwna implementacja konstruktora
///	explicit vector(size_type n,const T& val = T{}, const A&  a = A{}):
///		alloc_{a}//standardowe alokatory nie zgłaszają wyjątków
///	{
///		elem_ = alloc_.allocate(n);
		//możliwość zgłoszenia wyjątku o braku pamięci,czyli nie zostanie zaalokowana
		//żadna pamięć , jeszcze ok
///		space_ = last_ = elem_ + n;
///		for(T* p{elem_}; p != last_;++p)
///			a.construct(p,val);
				//konstruktor kopiujący może zgłosić wyjątek ,że nie można skopiować val
//				//tutaj już jest zajęta część pamięci , którą trzeba zwolnić ,ale typ T też może być utworzony w połowie!
///	}
	//w przypadku zgłoszenia wyjątku w konstruktorze nie zostanie wywołany destruktor


	//poprawiona implementacja
///	explicit vector(size_type n,const T& val = T{}, const A&  a = A{}):
///		alloc_{a}
///	{
///		if(n != 0)
///			elem_ = alloc_.allocate(n);

		//można użyć
		//std::allocator<int> s;

///		iterator p;
		//p musi być poza blokiem try
		//by było dostępne w catch

///		try
///		{
///			iterator end = elem_ + n;
///			for(T* p{elem_}; p != end;++p)
///					a.construct(p,val);
///			last_ = space_ = p;
///		}
///		catch (...)
///		{
///			for(iterator q{elem_}; q != p ; ++q )
///				alloc_.destroy(q);
			//usuwanie utworzonych elementów
///			alloc_.deallocate(elem_,n);
			//zwalnianie pamięci
///			throw;
			//powiadomienie o błędzie
///     }
///	}
	//jednak ,by uprościć implementację ,należy skorzystać z std::unitialized_fill()
	//funkcja ta zapewnia silną gwarancję

	explicit vector(size_type n,const T& val = T{}, const A&  a = A{}):
		alloc_{a}
	{
		elem_ = alloc_.allocate(n);
		try
		{
			std::uninitialized_fill(elem_,elem_ + n,val);
			space_ = last_ = elem_ + n;
		}
		catch (...) //lub samemu ogarnięcie hierarchii wyjątków
		{
			alloc_.deallocate(elem_,n);
			throw ;
		}
	}
	vector(const vector& a);
	vector& operator= (const vector& a);

	vector(vector&& a);
	vector& operator= (vector&& a);

	~vector();

	size_type size() const
	{
		return space_ - elem_;
	}

	size_type capacity() const
	{
		return last_ - elem_;
	}

	void reserve(size_type n);

	void resize(size_type n,const T& = {});
	void push_back(const T&);
};
