#pragma once

//Pozwala zdefiniować mechanizm subskrypcji w celu powiadamiania obiektów o zdarzeniach
//dziejących się w obserwowanym obiekcie

//Podmiot - obiekt posiadający interesujący stan
//Publikujący - powiadamiający o zmianach
//Subskrybent - obiekty śledzące zmiany stanu nadawcy

/*Zastosowanie:
 * 1. Gdy zmiany jednego obiektu mogą wymagać zmiany w innych obiektach ,a konkretny
 * zestaw obiektów nie jest zawczasu znany lub ulega zmianom dynamicznie
 * 2. Gdy obiekty muszą obserwować przez jakiś czas inne obiekty
 */

/*Implementacja
 * 1.Podzielić konkrenty kod na część publikującą oraz na część subsrybującą
 * 2.Deklaracja interfejsu subskrybenta
 * 3.Deklaracja interfejsu publikującego wraz z metodami do dodawania oraz usuwania
 * subskrybenów. Musi ona współdziałać z subsrybentami jedynie przez interfejs subsrybenta
 * 4.Faktyczną listę subskrybentów można umieścić w klasie abstrakcyjnej wywodzącej się
 * z interfejsu publikującego -> rozszerzanie jej. Jeżeli traktuję tym wzorcem istniejącą
 * hierarchię klas ,to powinienem logikę subskrypcji umieścić w osobym obiekcie
 * 5.Tworzenie konkretnych klas publikujących
 * 6.Implementacja metod aktualizacji i powiadamiania. Dane można przekazywać jako
 * argument metody powiadamiania. Alternatywnie Powiadamiający może wysłać samego siebie
 * w celu udostępnienia informacji.
 * 7.Klient tworzy Publikujących i Subskrybentów
 */

/*Zalety:
 * 1.Spełnia Open/Closed Principle
 * 2.Można utworzyć związek pomiędzy obiektami w trakcie działania programu.
 */

/*Wada:
 * 1.Subskrybenci powiadamiani są w przypadkowej kolejności.
 */

/*Powiązania:
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
 *2.Różnica między Mediatorem ,a Obserwatorem jest trudna do uchwycenia
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
#include <list>
#include <string>

/*Zamiar: Definicja mechanizmu subskrypcji do powiadamiania wielu obiektów o zmianiach
 * w innych obiektach.
 */
#define interface class

class IObserver
{
public:
	virtual ~IObserver() {}
	virtual void update(const std::string& msg) = 0;
};

class ISubjcect
{
public:
	virtual ~ISubjcect() {}
	virtual void attach(IObserver* observer) = 0;
	virtual void deteach(IObserver* observer) = 0;
	virtual void notify() = 0;
};

/*Klasa Subject zawiera istotny stan i powiadamia obserwatorów ,gdy ten stan się zmieni
 */

class Subject:
		public ISubjcect
{
private:
	std::list<IObserver*> list_observer_;
	std::string message_;
public:
	// ISubjcect interface
public:
	virtual void attach(IObserver* observer) override
	{
		list_observer_.push_back(observer);
	}
	virtual void deteach(IObserver* observer) override
	{
		list_observer_.remove(observer);
	}
	virtual void notify() override
	{
		howManyObserver();
		for(auto& observer : list_observer_)
			observer->update(message_);

	}
	void createMessage(std::string msg = "Eempty")
	{
		this->message_ = msg;
		notify();
	}
	void howManyObserver() const
	{
		std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
	}
	/*Zwykle , logika subskrypcji jest tylko częścią tego , co Subject może
	 * w rzeczywistości. Klasa Subject zwykle przechowuje ważną logikę biznesową , która
	 * wyzwala powiadomienie kiedy coś ważnego się dzieje
	 */
	void someBusinessLogic()
	{
		this->message_ = "change message message";
		notify();
		std::cout << "I'm about to do some thing important\n";
	}
};

class Observer:
		public IObserver
{
private:
	std::string message_from_subject_;
	Subject& subject_;
	static int static_number_;
	int number_;
	// IObserver interface
public:
	Observer(Subject& sub):
		subject_{sub}
	{
		this->subject_.attach(this);
		std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
		this->number_ = static_number_;
	}
	virtual ~Observer()
	{
		 std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
	}
	virtual void update(const std::string& msg) override
	{
		message_from_subject_ = msg;
		printInfo();

	}
	void printInfo()
	{
		std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_subject_ << "\n";
	}
	void removeMeFromList()
	{
		subject_.deteach(this);
		std::cout << "Observer \"" << number_ << "\" removed from the list.\n";
	}
};

int Observer::static_number_ = 0;


void clientCode()
{
	Subject* s {new Subject};
	Observer* ob1 {new Observer{*s}};
	Observer* ob2 {new Observer{*s}};
	Observer* ob3 {new Observer{*s}};
	Observer* ob4;
	Observer* ob5;

	s->createMessage("HELLLLOOOO!");
	ob3->removeMeFromList();

	s->createMessage("HELLLLOOOO! 2");
	ob4->removeMeFromList();

	s->createMessage("HELLLLOOOO! 3");
	ob5->removeMeFromList();

	ob2->removeMeFromList();
	ob1->removeMeFromList();

	delete ob5;
	delete ob4;
	delete ob3;
	delete ob2;
	delete ob1;
	delete s;
}

void ob_main()
{
	clientCode();
}








