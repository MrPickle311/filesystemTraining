#pragma once

//Pozwala  przekazywać żądania wzdłuż łańcucha obiektów obsługujących.
//Otrzymawszy żądanie każdy z obiektów obsługujących decyduje o przetworzeniu
//żądania lub przekazaniu go do kolejnego obiektu obsługującego w łańcuchu

//Wzorzec można poznać na podstawie obecności behawioralnych metod jednej grupy
//obiektów  pośrednio wywołujących analogiczne metody w innych obiektach
//za pośrednictwem wspólnego interfejsu

///Zastoswanie:
/// 1.Gdy mój program może obsługiwać różne rodzaje żądań na różne sposoby,ale
/// dokładne typy żądań i ich sekwencji nie są wcześniej znane
/// 2.Gdy istotne jest uruchomienie wielu obiektów obsługujących w pewnej kolejności
/// 3.Łańcuch pozwala ustawić obiekty obsługujące i ich kolejność w czasie działania
/// programu

/*Implementacja:
 * 1.Deklaracja interfejsu obsługującego i metody obsgługującej żądania
 * 2.Utworzyć abstrakcyjną bazową klasę obiektu obsługującego, wywodzącą się z
 * interfejsu obiektu obsługującego.Klasa taka powinna zawierać pole przechowujące
 * odniesienie do kolejnego ogniwa łańcucha.W razie potrzeby modyfikacji łańcucha
 * w czasie działania programu ,muszę zdefiniować setter dla tego pola.Powinna
 * też zawierać w sobie domyślne zachowanie handlera
 * 3.Stworzyć ogniwa(klasy) łańcucha
 * 4.Klient może sam złożyć łańcuch ,albo otrzymać go przygotowanego przez metodę
 * fabryczną
 * 5.Obsłużyć następujące sytuacje:
 * Łańcuch zawiera tylko jedno ogniwo.
 * Niektóre żądania mogą nie dotrzeć do końca łańcucha.
 * Inne żądania mogą dotrzeć do końca łańcucha i nie zostać obsłużone.
 */

/*Zalety:
 * 1.Można ustalić porządek obsługi żądania
 * 2.Spełnia Single Responsibility Principle
 * 3.Spełnia Open/Closed Principle
 */

//Wada:
//Niektóre żądania mogą wcale nie zostać obsłużone.

/*Powiązania z innymi wzorcami:
 * 1.Łańcuch zobowiązań,Polecenie,Mediator,Obserwator dotyczą różnych sposobów na
 * łączenie nadawców z odbiorcami żądań:
 *
#include <iostream>
#include <string>
#include <vector>

/*
 * Klasa interfesjowa Handler deklaruje metody potrzebne do budowy łańcucha
 * zobowiązań, oraz do wykoynywania zapytań/żądań.
 */

class Handler
{
public:
	virtual ~Handler() {}
	virtual Handler* setNext(Handler* handler) = 0;
	virtual std::string handle(std::string reqest) = 0;
};

//Domyślne zachowanie łańcuchowe może być zaimplementowane w klasie bazowej

//Ta klasa definiuje domyślne zachowanie
class AbstractHandler:
		public Handler
{
private:
	Handler* nextHandler_;
	// Handler interface
public:
	virtual ~AbstractHandler() {}
	virtual Handler* setNext(Handler* handler) override
	{
		this->nextHandler_ = handler;
		///Zwrócenie handlera pozwala na budowanie łańcucha w miejscu np. coś takiego
		/// $monkey->setNext($squirrel)->setNext($dog);
		return handler;
	}
	virtual std::string handle(std::string reqest) override
	{
		if(this->nextHandler_)
			return this->nextHandler_->handle(reqest);
		return {};
	}
};

///Wszystkie konkretne handlery zarówno przechwytują żądania jak i przekazują je
/// do następnego elementa łańcucha

class MonkeyHandler:
		public AbstractHandler
{
	// Handler interface
public:
	virtual ~MonkeyHandler() {}
	virtual std::string handle(std::string reqest) override
	{
		if (reqest == "Banana")
			return "Monkey: I'll eat the " + reqest + '\n';
		else return AbstractHandler::handle(reqest);
	}
};

class Squirellandler:
		public AbstractHandler
{
	// Handler interface
public:
	virtual ~Squirellandler() {}
	virtual std::string handle(std::string reqest) override
	{
		if (reqest == "Nut")
			return "Squirell: I'll eat the " + reqest + '\n';
		else return AbstractHandler::handle(reqest);
	}
};

class DogHandler:
		public AbstractHandler
{
	// Handler interface
public:
	virtual ~DogHandler () {}
	virtual std::string handle(std::string reqest) override
	{
		if (reqest == "MeatBall")
			return "Dog: I'll eat the " + reqest + '\n';
		else return AbstractHandler::handle(reqest);
	}
};

void clientCode(Handler* handler)
{
	std::vector<std::string> food {"Banana","Nut","Cup of coffee"};
	for(const std::string& f : food)
	{
		std::cout << "Client : Who wants a " << f << '\n';
		std::string const result {handler->handle(f)};
		if(!result.empty())
			std::cout << "  " << result;
		else std::cout << "  " << f << " was left untouched\n";
	}
}

//Inna część kodu klienckiego konstruuje łańcuch

void cr_main()
{
	MonkeyHandler* m {new MonkeyHandler};
	Squirellandler* s {new Squirellandler};
	DogHandler* d {new DogHandler};

	m->setNext(s)->setNext(d);

	//Klient powinien być w stanie wysłać żądanie do każdego handlera , nie
	//tylko pierwszego w łańcuchu, mogę zacząć w dowolnym miejscu łańcucha

	std::cout << "Chain: Monkey > Squirrel > Dog\n\n";

	clientCode(m);
	std::cout << "\n";
	std::cout << "Subchain: Squirrel > Dog\n\n";
	clientCode(s);

	delete m;
	delete s;
	delete d;
}
