#pragma once

//Polecenie zmienia żądanie w samodzielny obiekt zawierający wszystkie informacje o tym
//żądaniu.Parametryzowanie metod przy użyciu różnych żądań.Możliwość opóźniania lub
//kolejkowania żądań,czy ich cofanie

//Można go poznać po obecności zestawu klas odpowiadających konkretnym czynnościom
//implementującym ten sam interfejs.

/*Zastosowanie:
 * 1.Gdy chcę parametryzować obiekty za pomocą działań
 * 2.Gdy chcę układać kolejki zadań, bądź uruchamiać je zdalnie.Można serializować
 * polecenia do stringa i tak je przesyłać
 * 3.Gdy chcę zaimplementować operacje odwracalne.Wymaga to historii poleceń
 */

/*Implementacja:
 * 1.Deklaracja interfejsu polecenia z pojedynczą metodą uruchamiającą
 * 2.Ekstrakcja żądań do konkretnych ,odrębnych klas poleceń implementujących
 * interfejs polecenia.Każda klasa powinna mieć zestaw pól służących przechowywaniu
 * argumentów żądania wraz z odniesieniem do faktycznego obiektu odbiorcy. Konstrutkor
 * powinien inicjalizować te pola
 * 3.Dodanie pól Poleceń klasom Receiverów.
 * 4.Klient powinien inicjalizować obiekty w następującej kolejności:
 * - tworzyć Receiverów
 * - tworzyć Polecenia i kojarzyć je z Receiverami
 * - tworzyć invokerów i kojarzyć ich z Poleceniami
 */

/*Zalety:
 * 1.Spełnia Single Responsibility Principle
 * 2.SPełnia Open/Closed Principle
 * 3.Pozwala implementować undo/redo
 * 4.Pozwala opóźniać zadania
 * 5.Można złożyć zestaw prostszych poleceń w jedno złożone
 */

//Wada
//Komplikacja kodu

/*Powiązania z innymi wzorcami
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
 * 2.Obsługujący w Łańuchu zobowiązań mogą być zaimplementowani jako Polecenia. Można
 *wówczas wykonać wiele różnych działań reprezentowanych jako żądania na tym samym
 *obiekcie-kontekście.Można jeszcze to zrealizować tak,że samo żądanie jest Poleceniem
 *W takim przypadku można wykonać to samo działanie na łańcuchu różnych kontekstów
 * 3.Polecenie i Strategia wydają się podobne,gdyż parametryzują obiekt jakimś działaniem
 *Mają jednak inne cele:
 *-Polecenie: konwersja działania na obiekt.Parametry działania stają się polami tego obiektu
 *-Strategia: opisuje różne sposoby wykonywania danej czynności, pozwalając zamieniać algorytmy
 *w ramach jednej klasy kontekstu.
 * 4.Prototyp może pomóc stworzyć historię,zapisując kopie Poleceń
 * 5.Wizytator można traktować jako potężniejszą wersję Polecenia.Jego obiekty(wizytatora)
 *mogą wykonywać różne polecenia na obiektach różnych klas
 */

#include <string>
#include <iostream>

//Interfejs Command deklaruje metodę do odpalania Polecenia

class Command
{
public:
	virtual ~Command() {}
	virtual void execute() const = 0;
};

//Kilka poleceń ma możliwość implementacji prostych operacji samemu

class SimpleCommand:
		public Command
{
private:
	std::string pay_load_;
public:
	explicit SimpleCommand(std::string pay_load):
		pay_load_{pay_load}
	{}
	// Command interface
public:
	virtual void execute() const override
	{
		std::cout << "SimpleCommand: See, I can do simple things like printing ("
				  << pay_load_ << ")\n";
	}
};

/*
 * Klasy Receiver zawierają pewną logikę biznesową. Wiedzą jak wykonywać wszystkie
 * rodzaje operacji ,które są związane z przychodzącym żądaniem. Każda klasa może być
 * Receiverem
 */

class Receiver
{
public:
	void doSomething(const std::string& a)
	{
		std::cout << "Receiver: Working on (" << a << ".)\n";
	}
	void doSomethingElse(const std::string& b)
	{
		std::cout << "Receiver: Also working on (" << b << ".)\n";
	}
};

/*
 * Jednak niektóre polecenia mogą delegować bardziej skomplikowane operacje do innych
 * obiektów zwanych Receveirami
 */

class ComplexCommand:
		public Command
{
private:
	Receiver* receiver_;
	//Dane kontekstowe wymgane do opdalenia metod Receivera
	std::string a_;
	std::string b_;
	/*
	 * Złożone polecenia mogą akceptować jeden lub kilka obiektów-Receiverów wraz z
	 * przekazaniem danych kontekstowych poprzez konstruktor
	 */
public:
	ComplexCommand(Receiver* receiver,
				   std::string a,
				   std::string b):
		receiver_{receiver},
		a_{a},
		b_{b}
	{}

	// Command interface
	virtual void execute() const override
	{
		std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
		this->receiver_->doSomething(this->a_);
		this->receiver_->doSomethingElse(this->b_);
	}
};

/*
 * Invoker jest powiązany z jedną lub jedną lub kilkoma poleceniami. Wysyła on żądanie
 * do polecenia
 */

class Invoker
{
private:
	Command* on_start_;
	Command* on_finish_;
public:
	~Invoker()
	{
		delete  on_start_;
		delete  on_finish_;
	}
	void setOnStart(Command* command)
	{
		this->on_start_ = command;
	}

	void setOnFinish(Command* command)
	{
		on_finish_ = command;
	}
	/*
	 * Klasa Invoker nie zależy od konkretnego polecenia lub Receivera. Invoker przekazuje
	 * żądanie do Receivera pośrednio, odpalajac polecenie
	 */
	void doSomethinImportang()
	{
		std::cout << "Invoker: Does anybody want something done before I begin?\n";
		if (this->on_start_)
			this->on_start_->execute();
		std::cout << "Invoker: ...doing something really important...\n";
		std::cout << "Invoker: Does anybody want something done after I finish?\n";
		if (this->on_finish_)
			this->on_finish_->execute();
	}
};


//Kod kliencki parametryzuje Invokera jakimś poleceniem

void cc_main()
{
	Invoker* invoker {new Invoker};
	invoker->setOnStart(new SimpleCommand{"Say Hi!"});
	Receiver* receiver{new Receiver};
	invoker->setOnFinish(new ComplexCommand{receiver,"Send email", "Save report"});
	invoker->doSomethinImportang();

	delete invoker;
	delete receiver;
}










