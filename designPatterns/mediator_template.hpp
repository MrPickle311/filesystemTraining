#pragma once
/* Pozwala zredukować chaos zależności pomiędzy obiektami. Ogranicza bezpośrednią
 * komunikację pomiędzy obiektami i zmusza je do współpracy za pośrednictwem obiektu
 * mediatora
 */

/*Zastosowanie:
 * 1.Gdy zmiana jakichś klas jest trudna z powodu sprzęgnięcia z innymi klasami
 * 2.Gdy nie mogę ponownie użyć komponentu w innym programie ,z powodu zbytniej jego
 * zależności od innych komponentów
 * 3.Gdy zacznę tworzyć mnóstwo podklas komponentu tylko aby moć ponownie użyć jakieś
 * zachowanie w innych kontekstach
 */

/*Implementacja:
 * 1.Lokalizacja sprzężonych klas
 * 2.Deklaracja interfejsu mediatora
 * 3.Implementacja mediatora ,pamiętaj by przechowywała ona odniesienia do innych
 * komponentów
 * 4.Można klasę mediatora uczynić odpowiedzialną za niszczenie i kreację obiektów
 * 5.Komponenty powinny przechowywać odniesienie do obiektu mediatora.
 * 6.Komponenty powinny powiadamiać mediatora , a nie siebie nawzajem
 */

/*Zalety:
 * 1.Spełnia Single Responsibility Principle
 * 2.Spełnia Open/Closed Principle
 * 3.Można zredukować sprzężenie między różnymi komponentami programu
 * 4.Można ułatwić ponowne wykorzystanie komponentów
 */

/*Wada:
 * Z czasem Mediator może ewoluować do postaci Boskiego Obiektu
 */

/*Powiązania z innymi klasami:
 * 1.Łańcuch zobowiązań,Polecenie,Mediator,Obserwator dotyczą różnych sposobów na
 *łączenie nadawców z odbiorcami żądań:
 *(*) Łańcuch zobowiązań przekazuje żadanie sekwencyjnie wzdłuż dynamicznego łańcucha
 *potencjalnych odbiorców ,aż któryś je obsłuży
 *(*) Polecenie pozwala nawiązywać jednokierunkowe połączenia pomiędzy nadawcami i
 *odbiorcami
 *(*) Mediator eliminuje bezpośrednie połączenia pomiędzy nadawcami ,a odbiorcami ,
 *zmuszając ich do komunikacji za pośrednictwem obiektu mediator
 *(*) Obserwator pozwala odbiorcom dynamicznie zasubskrybować i zrezygnować z
 *subskrypcji żądań
 * 2.Fasada i Mediator maja podobne zadania: sluza zorganizowaniu wspolpracy miedzy
 *wieloma sciesle sprzegnietymi klasami
 *Fasada definiuje nowy interfejs ,lecz nie wprowadza nowych funkcjonalnosci.
 *Podsystem jest nieswiadomy instnienia fasady. Obiekty w obrębie podsystemu
 *mogą komunikować się bezpośrednio.
 *Mediator centralizuje komunikacje pomiedzy komponentami podsystemu. Komponenty
 *wiedza o obiekcie mediator i nie komunikuja sie ze soba bezposrednio
 * 3.Różnica między Mediatorem ,a Obserwatorem jest trudna do uchwycenia
 *Mediator -> Eliminacja zależności między zestawem komponentów systemu.Zamiast tego uzależnia się
 *te komponenty od jednego obiektu-mediatora.
 *Obserwator -> Ustanowienie dynamicznych ,jednokierunkowych połączneń między obiektami
 *,których część jest podległa innym.
 *Wzorce obserwatora oraz Mediatora można połączyć z sobą , tak ,by wszystkie komponenty
 *miały możliwość na dynamiczne połączenia między sobą. Nie będzie wówczas scentralizowanego
 *obiektu mediatora, a tylko rozproszony zestaw obserwatorów.
 *Obiekt mediatora pełni w niej rolę publikującego, zaś komponenty są subskrybentami
 *mogącymi “prenumerować” zdarzenia które nadaje mediator.Chodzi o systematyczne nadawanie
 *sygnałów/zdarzeń
 */

#include <iostream>
#include <string>

/*Interfejs Mediatora deklaruje metodę używaną przez komponenty ,by powiadamiać go
 * o różnych zdarzeniach. Mediator może reagować na nie i przekazać skutek ich wywołań
 * do innych komponentów.
 */

class BaseComponent;

class Mediator
{
public:
	virtual ~Mediator() {}
	virtual void notify(BaseComponent* sender,std::string event) const = 0;
};

/*Klasa BaseComponent zapewnia bazową funkcjonalność przechowywania instancji Mediatora
 * wewnątrz obiektów Komponentów
 */

class BaseComponent
{
protected:
	Mediator* mediator_;
public:
	BaseComponent(Mediator* mediator = nullptr):
		mediator_{mediator}
	{}
	void setMediator(Mediator* mediator)
	{
		this->mediator_ = mediator;
	}
};

/*Konkretne Komponenty implementują różne funkcjonalności, nie zależą od innych Komponentów,
 * ani od innych Mediatorów
 */

class Component1:
		public BaseComponent
{
public:
	void doA()
	{
		std::cout << "Component 1 does A.\n";
		this->mediator_->notify(this,"A");
	}
	void doB()
	{
		std::cout << "Component 1 does B.\n";
		this->mediator_->notify(this,"B");
	}
};

class Component2:
		public BaseComponent
{
public:
	void doC()
	{
		std::cout << "Component 1 does C.\n";
		this->mediator_->notify(this,"C");
	}
	void doD()
	{
		std::cout << "Component 1 does D.\n";
		this->mediator_->notify(this,"D");
	}
};

/*Konkretni Mediatorzy implementują wspólny wzorzec zachowań ,dla odpowiadających
 *  kilku Komponentów.
 */

class ConcreteMediator:
		public Mediator
{
private:
	Component1* comp1_;
	Component2* comp2_;
public:
	ConcreteMediator(Component1* c1,Component2* c2):
		comp1_{c1},
		comp2_{c2}
	{
		this->comp1_->setMediator(this);
		this->comp2_->setMediator(this);
	}
	// Mediator interface
public:
	virtual void notify(BaseComponent* sender, std::string event) const override
	{
		if(event == "A")
		{
			std::cout << "Mediator reacts on A and triggers following operations:\n";
			this->comp2_->doC();
		}
		if(event == "D")
		{
			std::cout << "Mediator reacts on D and triggers following operations:\n";
			this->comp1_->doB();
			this->comp2_->doC();
		}
	}
};

void clientCode()
{
	Component1* c1{new Component1};
	Component2* c2{new Component2};
	ConcreteMediator* mediator{new ConcreteMediator{c1,c2}};
	std::cout << "Client triggers operation A.\n";
	c1->doA();
	std::cout << "\n";
	std::cout << "Client triggers operation D.\n";
	c2->doD();

	delete c1;
	delete c2;
	delete  mediator;
}

void main_med()
{
	clientCode();
}
