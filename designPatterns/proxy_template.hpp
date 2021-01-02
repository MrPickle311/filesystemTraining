#pragma once

///Pozwala na stworzenie obiektu zastepczego w  miejsce innego obiektu.
///Nadzoruje dostep pierwotnego obiektu , pozwalajac na wykonanie jakiejs
///czynnosci przed lub po przekazaniu do niego rzadania(powinno byc zadania,
/// ale nie ma polskich liter)

///Pełnomocnik przechwytuje żądania od klienta,wykonuja jakąś pracę , a
///następnie przekazuje żądanie usługodawcy

///Pełnomocnik ma ten sam interfejs co usługodawca

//Jest niezastąpiony wszędzie tam ,gdzie trzeba dodać jakąś funkcjonalność
//obiektowi istniejącej klasy bez zmiany kodu klienta

///Zastosowanie:
/// 1.Leniwa inicjalizacja
/// 2.Kontrola dostępu. Np. gdy usługi stanowią część systemu operacyjnego,a
/// klienci to różne uruchamiane aplikacje
/// 3.Lokalne uruchamianie zdalnej usługi(pełnomocnik zdalny)
/// 4.Prowadzenie dziennika żądań(pełnomocnik prowadzący dziennik.
/// 5.Przechowywanie w pamięci podręcznej wyników działań. Pozwala zarządzać
/// cyklem życia pamięci
/// 6.Sprytne referencje.Można zlikwidować zasobożerny obiekt , do którego
/// nie ma żadnych referencji

///Implementacja:
/// 1. Stworzenie interfejsu usługi/obiektu lub Pełnomocnik dziedziczy z usługi
/// 2. Tworzenie klasy pełnomocnika. Powinna posiadać pole referujące do
/// usługi.
/// 3.Implementacja przeciążonych metod w klasie Pełnomocnika ,ale później
/// Pełnomocnik pozwala wykonać swoje zadanie w usłudze
/// 4.Rozważ utworzenie metody kreacyjnej która decyduje o tym, czy klient
/// otrzyma obiekt pełnomocnika, czy faktyczny obiekt usługi. Może to być
/// prosta, statyczna metoda w klasie pełnomocnika, albo w pełni rozwinięta
/// metoda wytwórcza.
/// 5.Przemyśleć sens istnienia leniwej inicjalizacji

/*Zalety:
 * 1.Można sterować obiektem usługi bez wiedzy klientów
 * 2.Można zarządzać cyklem życia obiektu usługi
 * 3.Pełnomocnik działa nawet wtedy, gdy obiekt udostępniający usługę nie jest
 * jeszcze gotowy
 * 4.Spełnia Open/Closed Principle
 */

/*Wady:
 * 1.Komplikacja kodu
 * 2.Odpowiedzi ze strony usługi mogą ulec opóźnieniu
 */

/*Powiązania z innymi wzorcami
   1.Adapter wyposaza opakowany obiekt w inny interfejs. Pelnomocnik w taki sam
	,zas Dekorator wprowadza rozszerzony interfejs
	2.Fasada przypomina Pelnomocnika ,oba buforuja zlozony podmiot i inicjalizuja
	go samodzielnie.Pelnomocnik jednak ma taki sam interfejs jak obiekt udostepniajacy
	usluge ,ktory ja reprezentuja ,co czyni je wymienialnymi
	3.Dekorator i Pelnomocnik maja podobne struktury ,ale inne cele. Oba wzorce bazuja
	na zasadzie kompozycji - jeden obiekt deleguje czesc zadan innemu.Pelnomocnik dodatkowo
	zarzadza cyklem zycia obiektu udostepniajacego jakas usluge , zas komponowanie Dekoratorow
	lezy w gestii klienta

 */
#include <iostream>
#include <string>

///Interfejs Subject deklaruje wspólne operacje dla obydwu RealSubject oraz
///dla Pełnomocnika. Tak długo jak klient pracuje z RealSubject używając
///tego interfejsu będę w stanie przekazać Pełnomocnika zamiast prawdziwego
///obiektu

class Subject
{
public:
	virtual void Request() const = 0;
};

///RealSubject zawiera pewną logikę biznesową. Klasa ta jest zdolna do wykonania
/// użytecznej pracy ,która może być wrażliwa na błędy lub powolna. Pełnomocnik
/// może rozwiązać te problemy bez żadnych zmian kodu klasy RealSubject

class RealSubject:
		public Subject
{
	// Subject interface
public:
	virtual ~RealSubject() {}
	virtual void Request() const override
	{
		std::cout << "RealSubject: Handling request.\n" << std::endl;
	}
};

//Pełnomocnik posiada taki sam interfejs jak RealSubject

class Proxy:
		public Subject
{
private:
	RealSubject* rs_;

	bool checkAccess() const
	{
		std::cout << "Proxy: Checking access prior to firing a real request.\n";
		return true;
	}

	void logAccess() const
	{
		std::cout << "Proxy: Logging the time of request.\n";
	}
	///Pełnomocnik zarządza referencją do obiektu RealSubject. Może być
	/// wstawiona przez kompozyjcę lub przekazywany jako argument konstruktora
	// Subject interface
public:
	Proxy(RealSubject* rs):
		rs_{new RealSubject{*rs}}
	{}
	virtual ~Proxy()
	{
		delete  rs_;
	}

	///Pełnomocnik może zapewnnić/wykonać jakieś operacje
	/// (podane w zastosowaniach) samemu oraz resztę oddelegować do
	/// obiektu RealSubject
	virtual void Request() const override
	{
		if (this->checkAccess())
		{
			this->rs_->Request();
			this->logAccess();
		}
	}
};

///Kod klienta pracować może z prawdziwymi obiektami oraz z ich Pełnomocnikami
/// poprzed wspólny interfejs. Czasem też można po prostu dziedziczyć po
/// prawdziwej klasie
void clientCode(const Subject& subject)
{
	subject.Request();
}

void pr_main()
{
	std::cout << "Client: Executing the client code with a real subject:\n";
	RealSubject* real_subject {new RealSubject};
	clientCode(*real_subject);
	std::cout << "\n";
	std::cout << "Client: Executing the same client code with a proxy:\n";
	Proxy* proxy {new Proxy{real_subject}};
	clientCode(*proxy);

	delete proxy;
	delete real_subject;
}
