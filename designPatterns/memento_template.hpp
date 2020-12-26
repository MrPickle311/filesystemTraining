#pragma once

//Pamiątka pozwala zapisywać i przywracać wczesniejszy stan obiektu be z ujawniania
//szczegółów jego implementacji


#include <iostream>
#include <string>
#include <chrono>
#include <vector>

/*Interfejs Pamiątki zapewnia sposób na odzyskanie jego danych. Jednakże
 * nie eksponuje on stanu obiektu przechowywanego
 */

/*Zastosowanie:
 * 1.Do tworzenia migawek obiektów i przywracania ich stanów
 * 2.Gdy chcę znacząco poprawić hermetyzajcę
 */

/*Implementacja:
 * 1.Określ klasę/klasy źródła
 * 2.Stworzenie Pamiątki
 * 3.Pamiątka nie może mieć setterów,powinna przyjmować dane tylko raz
 * 4.Zagnieździć klasę pamiątki w klasie źródła
 * 5.Dodaj do klasy źródła metody save() oraz restore()
 * 6.Zarządca ( Caretaker ) powinien obsługiwać cały system tworzenia i przywracania
 * stanów(kiedy,co,jak,gdzie) oraz sposoby i obsługę przechowywania
 * 7.Jeśli Pamiątka jest zagnieżdżona lub klasa źródła udostępnia funkcje setter ,to
 * połączenie między zarządcami ,a źródłami można prznieść do Pamiątki -> każda
 * Pamiątka musi być połączona ze źródłem ,które je utworzyło
 */

/*Zalety:
 * 1.Można tworzyć migawki obiektów bez naruszania hermetyzacji
 * 2.Można uprościć kod źródła,pozwalając śledzić historię stanu źródła
 */

/*Wady:
 * 1.Żre RAM
 * 2.Zarządcy powinni śledzić cykl życia źródła, aby być w
 * stanie kasować zbędne pamiątki.
 * 3.Większość dynamicznych języków programowania, jak PHP,
 * Python i JavaScript nie daje gwarancji niezmienialności stanu pamiątki.
 */

/*Powiązania:
 * 1.Można stosować Pamiątkę i Polecenie jednocześnie implementując funkcjonalność "cofnij"
 * Polecenia -> dokonują działań na obiekcie-źródła
 * Pamiątka -> przechowuje stan
 * 2.Można zastosować Pamiątkę wraz z Iteratorem by zapisać bieżący stan iteracji,co
 * pozwoli w razie czego do niego wrócić
 * 3.Prototyp czasem moze byc prostsza alternatywa dla Pamiatki. Jest to mozliwe
 * gdy obiekt przechowywany jest nieskomplikowany.Obiekt taki nie
 * powinien miec powiazan z zewnetrznymi zasobami
 */

class Memento
{
public:
	virtual std::string getName() const = 0;
	virtual std::string date() const = 0;
	virtual std::string state() const = 0;
};

/*Klasa ConcreteMemento zawiera infrastrukturę dla przechowywania
 * stanu obiektu przechowywanego
 */

class ConcreteMemento:
		public Memento
{
private:
	std::string state_;
	std::string date_;
public:
	ConcreteMemento(std::string state):
		state_{state}
	{
		std::time_t now {std::time(0)};
		this->date_ = std::ctime(&now);
	}
	// Memento interface
public:
	virtual std::string getName() const override
	{
		return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
	}
	virtual std::string date() const override
	{
		return date_;
	}
	virtual std::string state() const override
	{
		return state_;
	}
};

/* Klasa Originator(obiekt przechowywany) zawiera istotne dane zmieniające się w czasie.
 * Zawiera również kilka metod do zapisywania stanu wewnątrz Pamiątki oraz inne metody
 * do przywracania swojego stanu z Pamiątki.
 */

class Originator
{
private:
	std::string state_; //taka sobie ogólna reprezentacja stanu
private:
	std::string generateRandomString(int length = 10)
	{
		const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
		int stringLength {sizeof (alphanum) - 1};

		std::string random_string;
		for(int i{0}; i < length ; ++i)
			random_string += alphanum[std::rand() % stringLength];

		return random_string;
	}
public:
	Originator(std::string state):
		state_{state}
	{
		std::cout << "Originator: My initial state is: " << this->state_ << "\n";
	}
	/*Logika biznesowa Orginatora może wpływać na wewntętrzny stan. W związku z tym
	 * klient może powinien przeprowadzać backup stanu przed uruchomieniem metod
	 * logiki biznesowej poprzez save()
	 */
	void doSth()
	{
		std::cout << "Originator: I'm doing something important.\n";
		this->state_ = generateRandomString(30);
		std::cout << "Originator: and my state has changed to: " << this->state_ << "\n";
	}
	//Zapisuje obecny stan do Pamiątki
	Memento* save()
	{
		return new ConcreteMemento{this->state_};
	}
	//przeprowadzanie backup'u
	void restore(Memento* m)
	{
		this->state_ = m->state();
		std::cout << "Originator: My state has changed to: " << this->state_ << "\n";
	}
};

/*Klasa Caretaker nie zależy od konkretnej klasy Pamiątki.W związku z tym , nie ma
 * dostępu do stanu Originator'a przechowywanego w Pamiątce. Pracuje ze wszystkimi
 * Pamiątkami poprzez interfejs Pamiątki
 */

class Caretaker
{
private:
	std::vector<Memento*> mementos_;
	Originator* originator_;
public:
	Caretaker(Originator* ori):
		originator_{ori}
	{}

	void backup()
	{
		std::cout << "\nCaretaker: Saving Originator's state...\n";
		this->mementos_.push_back(this->originator_->save());
	}

	void undo()
	{
		if(this->mementos_.empty())
			return;
		Memento* memento = this->mementos_.back();
		this->mementos_.pop_back();
		std::cout << "Caretaker: Restoring state to: " << memento->getName() << "\n";
		try
		{
			this->originator_->restore(memento);
		}
		catch (...)
		{
			this->undo();
		}
	}

	void showHistory() const
	{
		std::cout << "Caretaker: Here's the list of mementos:\n";
		for(auto m : mementos_)
			std::cout << m->getName() << '\n';
	}
};

void clientCode()
{
	Originator* org {new Originator{"Extra state"}};
	Caretaker* crtk {new Caretaker{org}};
	crtk->backup();
	org->doSth();
	crtk->backup();
	org->doSth();
	crtk->backup();
	org->doSth();
	std::cout << '\n';
	crtk->showHistory();
	std::cout << "\nClient: Now, let's rollback!\n\n";
	crtk->undo();
	std::cout << "\nClient: Once more!\n\n";
	crtk->undo();
	delete  org;
	delete  crtk;
}

void mem_main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	clientCode();
}





























