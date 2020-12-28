#pragma once

/*Pozwala obiektowi zmienic swoje zachowanie ,gdy zmieni się jego stan wewnętrzny.
 * Wygląda to tak , jakby obiekt zmienił swoją klasę
 */

/*Zastosowanie:
 * 1.Gdy mam do czynienia z obiektem ,którego zachowanie jest zależne od jego stanu
 * ,liczba stanów jest wielka , a kod specyficzny dla danego stanu często ulega zmianom
 * 2.Gdy mam kod zaśmiecony ifami i switchami
 * 3.Redukcja dużej ilości kodu powtarzającego się w wielu stanach
 */

/*Implementacja:
 * 1.Znalezienie klasy pełniącej rolę kontekstu.
 * 2.Deklaracja interfejsu stanu. Musi deklaraować metody odzwierciedlające stan
 * kontekstu
 * 3.Implementacja konkretnych stanów.Ekstrakcja kodu z kontekstu , który dotyczy stanu.
 * Jeśli stan zależy od prywatnych pól/metod klasy ,to:
 * -Można je uczynić publicznymi
 * -Można zagnieździć klasy stanów w klasie kontekstu
 * 4.Dodanie pola z odniesieniem do interfejsu stanu i publicznego settera dla tego pola
 * 5.Zamiana skomplikowanych ifów na stany
 */

/*Zalety:
 * 1.Spełnia Single Responsibility Principle
 * 2.Spełnia Open/Closed Principle
 * 3.Upraszcza kod kontekstu zamieniajac if-y i switch-e z na stany
 */

/*Wada:
 * Zastosowanie tego wzorca może być przesadą jeśli mamy do czynienia
 *  zaledwie z kilkoma stanami i rzadkimi zmianami.
 */

/*Powiązania
 * 1.Most,Stan,Strategia i pewnym stopniu Adapter maja podobna strukture.
 * Wszystkie oparte sa na kompozycji ,co oznacza delegowanie zadan innym
 * obiektom . Jednak kazdy z tych wzorcow rozwiazuje inne problemy. Wzorzec
 * nie jest bowiem tylko recepta na ustrukturyzwanie kodu w pewien sposob,lecz
 * takze informacja dla innych deweloperow o charakterze rozwiazywanego problemu
 * 2.Stan można uważać za rozszerzenie Strategii. Oba wzorce oparte są o kompozycję
 * ; zmieniają zachowanie kontekstu przez delegowanie części zadań obiektom pomocniczym.
 * Strategia czyni te obiekty całkowicie niezależnymi i nieświadomymi siebie nawzajem.
 * Stan nie ogranicza zależności między konkretnymi stanami i pozwala im zmieniać stan
 * kontekstu według uznania
 */



#include <iostream>
#include <typeinfo>

/* Klasa bazowa State deklaruje metody dla Concrete States oraz zapewnia wsteczną
 * referencję do obiektu Context , powiązanego ze stanem. Ta referencja może być
 * użyta w celu zmiany stanu obiektu Context.
 */

class Context;

class State
{
protected:
	Context* context_;
public:
	virtual ~State() {}

	void setContext(Context* context)
	{
		this->context_ = context;
	}

	virtual void handle1() = 0;
	virtual void handle2() = 0;
};

/*Klasa Context definiuje interfejs dla klientów. Również zarządza referencją do
 * instancji klasy State , która reprezentuje bieżący stan obiektu Context
 */

class Context
{
private:
	State* state_;
public:
	Context(State* state):
		state_{nullptr}
	{
		transitionTo(state);
	}
	~Context()
	{
		delete  state_;
	}
	//Klasa Context pozwala na zmianę obiektu State w czasie działania programu
	void transitionTo(State* state)
	{
		std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
		if(this->state_ != nullptr)
			delete state_;
		this->state_ = state;
		this->state_->setContext(this);
	}
	//Context deleguje część jego zachowania do obiektu Stanu
	void request1()
	{
		this->state_->handle1();
	}
	void request2()
	{
		this->state_->handle2();
	}
};

//Concrete States implementuja różne zachowania powiązane z stanem obiektu Context

class ConcreteStateA:
		public State
{
	// State interface
public:
	virtual void handle1() override;
	virtual void handle2() override
	{
		std::cout << "ConcreteStateA handles request2.\n";
	}
};

class ConcreteStateB:
		public State
{
	// State interface
public:
	virtual void handle1() override
	{
		std::cout << "ConcreteStateB handles request1.\n";
	}
	virtual void handle2() override
	{
		std::cout << "ConcreteStateB handles request2.\n";
		std::cout << "ConcreteStateB wants to change the state of the context.\n";
		this->context_->transitionTo(new ConcreteStateA);
	}
};


inline void ConcreteStateA::handle1()
{
	std::cout << "ConcreteStateA handles request1.\n";
	std::cout << "ConcreteStateA wants to change the state of the context.\n";
	this->context_->transitionTo(new ConcreteStateB);
}

void clientCode()
{
	Context* c {new Context{new ConcreteStateA}};
	c->request1();
	c->request2();
	delete c;
}

void st_main()
{
	clientCode();
}

