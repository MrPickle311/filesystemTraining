#pragma once

///Wyposaza biblioteke,framework lub inny zlozony zestaw klas w uproszczony interfejs

//Przydaje sie przy pracy ze zlozonymi bibliotekami i API

///Mozna ten wzorzec poznac po istnieniu klasy o uproszczonym interfejsie ,ktora
///deleguje wiekszosc swoich zadan innym klasom. Zwykle fasady zarzadzaja calym cyklem
/// zycia uzywanych przez siebie obiektow

///Zastosowanie
/// 1.Gdy potrzebuje ograniczonego ,ale latwego dostepu przy uzyciu interfejsu
/// do calego podsystemu
/// 2.Gdy chce ustrukturyzowac podsystem w warstwy

//dziel system na warstwy (np. dzwiek,video) i spraw ,by jego elementy porozumiewaly
//sie miedzy soba poprzez fasade

///Implementacja
/// 1.Jesli mozna uproscic istniejacy interfejs podsystemu lub uniezaleznic kod klienta
/// od naszych klas
/// 2.Stworzyc nowy interfejs fasady
/// 3.Kod kliencki powinien sie komunikowac z podsystemem tylko poprzez fasade,by byc
/// chronionym przed aktualizacjami podsystemu
/// 4.Jesli fasada sie rozrosnie ,to stworz nowa,rekurencyjnie

//Zaleta:
//Mozna odizolowac sie od zlozonosci systemu

//Wada:
//Fasada moze sie stac God Object sprzezonym ze wszystkimi klasami aplikacji.
//God Object - klasa ,ktora wie za duzo oraz robi za duzo

///Powiazania:
/// 1. Fasada definiuje nowy interfejs istniejacym obiektom ,zas Adapter zaklada
/// zwiekszenie uzytecznosci zostanego interfejsu. Adapter na ogol opakowuje
/// pojedynbczy obiekt ,zas Fasada obejmuje caly podsystem obiektow
/// 2.Fabryka abstrakcyjna moze sluzyc jako dobra alternatywa do
/// fasady gdy chcesz zrobic, to ukrycie przed kodem klienckim
/// procesu tworzenia obietkow
/// 3.Pylek przedstawia sposob na stworzenie wielkiej liczby malych
/// obiektow ,zas Fasada na stworzenie pojedynczego obiektu reprezentujacego
/// caly podsystem
/// 4.Fasada i Mediator maja podobne zadania: sluza zorganizowaniu wspolpracy miedzy
/// wieloma sciesle sprzegnietymi klasami
/// Fasada definiuje nowy interfejs ,lecz nie wprowadza nowych funkcjonalnosci.
/// Podsystem jest nieswiadomy instnienia fasady. Obiekty w obrębie podsystemu
/// mogą komunikować się bezpośrednio.
/// Mediator centralizuje komunikacje pomiedzy komponentami podsystemu. Komponenty
/// wiedza o obiekcie mediator i nie komunikuja sie ze soba bezposrednio
/// 5. Fasada moze byc przeksztalcona w Singleton
/// 6.Fasada przypomina Pelnomocnika ,oba buforuja zlozony podmiot i inicjalizuja
/// go samodzielnie.Pelnomocnik jednak ma taki sam interfejs jak obiekt udostepniajacy
/// usluge ,ktory ja reprezentuja ,co czyni je wymienialnymi

#include <string>
#include <iostream>

//Klasa Subsystem moze akceptowac zadania zarowno od strony fasady , jak i bezposrednio
//od klienta. Fasada jest tak jakby klientem , a nie czescia podsystemu

class Subsystem1
{
public:
	std::string operation1() const
	{
		return  " Subsystem1 OPERATION 1";
	}
	std::string operationN() const
	{
		return  " Subsystem1 OPERATION N";
	}
};

//Fasady moga wspolpracowac z wieloma podsystemami

class Subsystem2
{
public:
	std::string operation1() const
	{
		return  " Subsystem2 OPERATION 1";
	}
	std::string operationZ() const
	{
		return  " Subsystem2 OPERATION Z";
	}
};

///Klasa fasady zapewnia prosty interfejs zlozonej logiki jednego lub wiecej systemow.
/// Fasada deleguje zadania klienta do odpowiedniego obiektu podsystemu.
/// Fasady steruja cyklem zycia obiektow podsystemu

class Facade
{
protected:
	Subsystem1* s1_;
	Subsystem2* s2_;
	//Fasade mozna uzyc do istniejacego podsystemu lub usilowac zbudowanie go
	//za jej pomoca
public:
	Facade(Subsystem1* sub1 = nullptr,Subsystem2* sub2 = nullptr):
		s1_{sub1},s2_{sub2}
	{
		if(!s1_)
			sub1 = new Subsystem1;
		if(!s2_)
			sub2 = new Subsystem2;
	}
	~Facade()
	{
		delete s1_;
		delete s2_;
	}
	///Metody fasasdy sa skrotami zlozonej funkcjonalnosci podsystemu.
	std::string operation()
	{
		std::string result {"Facade initializes subsystems:\n"};
		result += this->s1_->operation1();
		result += this->s1_->operationN();
		result += "Facade orders subsystems to perform the action:\n";
		result += this->s2_->operation1();
		result += this->s2_->operationZ();
		return result;
	}
};


//Kod klienta dziala z calym zlozonym podsystemem poprzez prosty interfejs.
//Klient nie musi sterowac cyklem zycia podsystemu

void clientCode(Facade* f)
{
	std::cout << f->operation();
}

//Jednak klient moze samodzielnie tworzyc sobie elementy podsystemu i sterowac ich
//cyklem zycia

void f_main()
{
	Subsystem1* s1 { new Subsystem1};
	Subsystem2* s2 { new Subsystem2};
	Facade* facade {new Facade{s1,s2}};
	clientCode(facade);
	delete facade;
	std::cout << '\n';
}
