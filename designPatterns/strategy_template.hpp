#pragma once

//Pozwala zdefiniować rodzinę aglorytmów , umieścić je w osobnych klasach oraz
//uczynić obiekty tych klas wymienialnymi

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

/*Interfejs Strategy deklaruje operacje wspólne dla wszystkich wersji tego samego
 * algorytmu.
 * Klasa Context używa tego interfejsu by wywołać algorytm zdefiniowany przez
 * Concrete Strategies
 */

/*Zastosowanie:
 * 1.Gdy chcę używać jednego wariantu algorytmu w obrębie obiektu i zmieniać go podczas
 * działania programu
 * 2.Gdy mam w programie wiele podobnych klas,różniących się jedynie sposobem działania
 * 3.Izolacja logiki biznesowej od szczegółów implementacyjnych
 * 4.Gdy wybór odpowiedniego algorytmu siedzi w olbrzymim operatorze warunkowym
 */

/*Implementacja:
 * 1.Indentyfikacja algorytmu
 * 2.Deklaracja interfejsu strategii
 * 3.Ekstrakcja algorytmów
 * 4.Dodanie pola odniesienia do interfejsu strategii oraz settera doń
 */

/*Zalety:
 * 1. Możesz zamieniać algorytmy stosowane w obrębie obiektu w trakcie
 *  działania programu.
 * 2. Możesz odizolować szczegóły implementacyjne algorytmu od kodu
 * który z niego korzysta.
 * 3.Zamiana dziedziczenia na kompozycję
 * 4.Spełnia Open/Closed Principle
 */

/*Wady:
 * 1.Nie trzeba tego stosować dla zaledwie kilku aglorytmów
 * 2.Klienci muszą być świadomi różnicy między strategiami,gdyż oni je ustawiają
 * 3.Można zamiast tego wykorzystać funktory
 */

/*Powiązania:
 *1.Most,Stan,Strategia i pewnym stopniu Adapter maja podobna strukture.
 * Wszystkie oparte sa na kompozycji ,co oznacza delegowanie zadan innym
 * obiektom . Jednak kazdy z tych wzorcow rozwiazuje inne problemy. Wzorzec
 * nie jest bowiem tylko recepta na ustrukturyzwanie kodu w pewien sposob,lecz
 * takze informacja dla innych deweloperow o charakterze rozwiazywanego problemu
 * 2.Polecenie i Strategia wydają się podobne,gdyż parametryzują obiekt jakimś działaniem
 *Mają jednak inne cele:
 *-Polecenie: konwersja działania na obiekt.Parametry działania stają się polami tego obiektu
 *-Strategia: opisuje różne sposoby wykonywania danej czynności, pozwalając zamieniać algorytmy
 *w ramach jednej klasy kontekstu.
 *3.Dekorator pozwala zmienić otoczkę obieku ,zaś Strategia jego wnętrzne
 *4.Metoda szablonowa polega na mechanizmie dziedziczenia: pozwala zmieniać części
 *aglorytmu rozszerzając je w podklasach. Strategia bazuje na kompozycji :
 *można zmienić częśc zachowania obiektu nadanie mu różnych strategii.Metoda działa
 *na poziomie klasy-> statyczna. Strategia -> kompozycja ->runtime
 *5.Stan można uważać za rozszerzenie Strategii. Oba wzorce oparte są o kompozycję
 * ; zmieniają zachowanie kontekstu przez delegowanie części zadań obiektom pomocniczym.
 * Strategia czyni te obiekty całkowicie niezależnymi i nieświadomymi siebie nawzajem.
 * Stan nie ogranicza zależności między konkretnymi stanami i pozwala im zmieniać stan
 * kontekstu według uznania
 */
class Strategy
{
public:
	virtual ~Strategy() {}
	virtual std::string doAlgorithm(const std::vector<std::string>& data) const = 0;
};


class Context
{
	/* Ta klasa ma referencję do jednego obiektu strategii, kontekst nie zna konkretnej
	 * klasy strategii. Pracuje ona tylko z interfejsem Strategy
	 */
private:
	Strategy* strategy_;
	/*Zwykle klasa przyjmuje obiekt Strategy poprzez konstruktor, ale również udostępnia
	 * setter dla tego pola
	 */
public:
	Context(Strategy* str = nullptr):
		strategy_{str}
	{}
	~Context()
	{
		delete this->strategy_;
	}
	void setStrategy(Strategy* str)
	{
		delete this->strategy_;
		this->strategy_ = str;
	}
	void doSth() const
	{
		std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n";
		std::string res {this->strategy_->doAlgorithm(std::vector<std::string>{"a", "e", "c", "b", "d"})};
		std::cout << res << "\n";
	}
};

/*Concrete Strategies implementują algorytm Strategy.
 */

class ConcreteStrategyA:
		public Strategy
{
	// Strategy interface
public:
	virtual std::string doAlgorithm(const std::vector<std::string>& data) const override
	{
		std::string res;
		std::for_each(data.begin(),data.end(),[&res](const std::string& str)
		{
			res += str;
		});
		return res;
	}
};

class ConcreteStrategyB:
		public Strategy
{
	// Strategy interface
public:
	virtual std::string doAlgorithm(const std::vector<std::string>& data) const override
	{
		std::string res;
		std::for_each(data.begin(),data.end(),[&res](const std::string& str)
		{
			res += str;
		});
		for(size_t i{0}; i < res.size() / 2 ;++i)
			std::swap(res[i],res[res.size() - 1 - i]);
		return res;
	}
};

//Kod klienta samemu dobiera strategię do kontekstu

void clientCode()
{
	Context* c {new Context{new ConcreteStrategyA}};
	std::cout << "Client: Strategy is set to normal sorting.\n";
	c->doSth();
	std::cout << "\n";
	std::cout << "Client: Strategy is set to reverse sorting.\n";
	c->setStrategy(new ConcreteStrategyB);
	c->doSth();
	delete c;
}

void strat_main()
{
	clientCode();
}
