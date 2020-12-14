#pragma once

//pozwala zmiescic wiecej obiektow w danej przestrzeni pamieci RAM poprzez wspoldzielenie
//czesci opisu ich stanow

//dane niezmienne - stan wewnetrzny, obiekty maja do niego tylko prawo odczytu
//dane zmienne - stan zewnerzny

//Pylek - obiekt przechowujacy stan wewnetrzny
//mozna sie tutaj posluzyc metoda fabryczna lub fabryka

//Wzorzec ten sklada sie z nastepujacych klas:
//Pylek  - obiekt przechowujacy stan wewnetrzny, moze byc ich kilka unikatowych
//Konteks - zawiera opis zewnetrznego obiektu , unikalny dla pierwotnych obiektow
//Skojarzenie Konekstu i Pylku daje pelny obiekt
//Klient  - wiadomo
//Fabryka pylkow - zajmuje sie produkcja i pula Pylkow

//Pylek mozna poznac po obecnosci metody kreacyjnej zwracajacej obiekty z pamieci
//podrecznej zamiast nowo utworzonych

//Zastosowanie :
//Oszczedzanie pamieci RAM w przypadku praca z duza iloscia podobnych obiektow

///Implementacja:
/// 1.Podzielic pola klasy na stan zewnetrzny i wewnetrzny
/// 2.Pozostawic pola stanu wewnetrznego w klase ,ale nie moga byc zmieniane.
/// Powinny byc inicjalizowane w konstruktorze
/// 3.Przejrzyj metody korzystające z pól zewnętrznego stanu.
/// Dla każdego pola użytego w metodzie, dodaj nowy parametr i używaj go zamiast pola.
/// 4.Utworzyc klase fabryczna.Szuka pylku przed utworzeniem go. Jednyny interfejs
/// jaki mozna wystawic klientowi
/// 5.Klient musi konstruowac sam zewnetrzne stany,dla wygody zewnetrzny stan
/// mozna przeniesc do osobnej klasy kontekstowej

//Zaleta
//Oszczedzanie pamieci RAM

//Wady:
// 1.Może się zdarzyć, że oszczędność pamięci odbędzie się kosztem czasu procesora,
//gdyż część danych
//kontekstowych musi być wyliczana przy każdym wywołaniu metody pyłka.
//2. Kod staje się dużo bardziej skomplikowany.

///Powiazania:
/// 1.Liscie drzewa Kompozytowego mozna zaimplementowac jako Pylki
/// 2.Pylek przedstawia sposob na stworzenie wielkiej liczby malych
/// obiektow ,zas Fasada na stworzenie pojedynczego obiektu reprezentujacego
/// caly podsystem
/// 3.Pylek a Singleton.
/// Singleton: stan moze sie zmieniac, ale istnieje tylko jeden Singleton
/// Pylek: stan niezmienny , ale istniec moze wiele pylkow

#include <iostream>
#include <string>
#include <unordered_map>
//cos mi nie dziala , ale idea ok
struct SharedState
{
	std::string brand_;
	std::string model_;
	std::string color_;

	SharedState(const std::string& brand,const std::string& model,const std::string& color):
		brand_{brand},
		model_{model},
		color_{color}
	{}

	friend std::ostream& operator<< (std::ostream& os,const SharedState& ss)
	{
		os << ss.brand_ << '\n';
		os << ss.color_ << '\n';
		os << ss.model_ << '\n';
	}
};

struct UniqueState
{
	std::string owner_;
	std::string plates_;

	UniqueState(const std::string& owner,const std::string& plates):
		owner_{owner},
		plates_{plates}
	{}
	friend std::ostream& operator<< (std::ostream& os,const UniqueState& us)
	{
		os << us.owner_ << '\n';
		os << us.plates_ << '\n';
	}
};

///Klasa Flyweight przechowuje wspolna czesc stanu ,ktora nalezy do wielu rzeczywistych
///obiektow. Flyweight akceptuje reszte stanu(zewnetrznego) poprzez metody

class Flyweight
{
private:
	SharedState* shared_state_;
public:
	Flyweight(const SharedState*ss):
		shared_state_{new SharedState{*ss}}
	{}
	Flyweight(const Flyweight& other):
		shared_state_{new SharedState{*other.shared_state_}}
	{}
	~Flyweight()
	{
		delete  shared_state_;
	}
	SharedState* shared_state() const
	{
		return shared_state_;
	}
	void operation(const UniqueState& us) const
	{
		std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique ("
				  << us << "(state \n";
	}
};

///Fabryka pylkow tworzy i zarzadza pylkami. Zapewnia to ,ze pylki sa wspoldzielone
/// w sposob odpowiedni. Gdy klient poprosi o jakis pylek i go nie bedzie w bazie
/// to fabryka tworzy nowy pylek

class FlyweightFactory
{
private:
	std::unordered_map<std::string,Flyweight> flyweights_;

	//zwraca string-hash dla zadanego stanu
	std::string getKey(const SharedState& ss) const
	{
		return  ss.brand_ + "_" + ss.color_ + "_" + ss.model_;
	}

public:
	FlyweightFactory(std::initializer_list<SharedState> shared_states)
	{
		for (const SharedState& ss : shared_states )
			this->flyweights_.insert((std::make_pair(this->getKey(ss),Flyweight{&ss})));
	}
	//Zwraca istniejacy Pylek z zadanym stanem lub tworzy nowy

	Flyweight getFlyweight(const SharedState& ss)
	{
		std::string key {this->getKey(ss)};
		if (this->flyweights_.find(key) == this->flyweights_.end())
		{
			this->flyweights_.insert((std::make_pair(this->getKey(ss),Flyweight{&ss})));

		}
		else std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
		return this->flyweights_.at(key);
	}
	void listOfFlyweights() const
	{
		size_t count {this->flyweights_.size()};
		std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
		for(auto& x : this->flyweights_)
			std::cout << x.first << " " << std::endl;
	}
};


void addCarToPoliceDatabase(
		FlyweightFactory& ff,const std::string& plates,const std::string& owner,
		const std::string& brand,const std::string& model,const std::string& color)
{
	std::cout << "\nClient: Adding a car to database.\n";
	const Flyweight& flyweight {ff.getFlyweight({brand,model,color})};
	//Kod klienta zarowno magazynuje lub tworzy zewnetrzny stan i przekazuje to
	//to metod Pylka
	flyweight.operation({plates,owner});

}

///Kod klienta zwykle tworzy wstepnie wypelnione pylki w etapie inicjalizacji
/// aplikacji

void fl_main()
{
	FlyweightFactory* fac {new FlyweightFactory{
			{{"Chevrolet", "Camaro2018", "pink"}, {"Mercedes Benz", "C300", "black"},
				{"Mercedes Benz", "C500", "red"}, {"BMW", "M5", "red"},
				{"BMW", "X6", "white"}}
		}};
	addCarToPoliceDatabase(*fac,"CL234IR",
								"James Doe",
								"BMW",
								"X1",
								"red");
	addCarToPoliceDatabase(*fac,
								"CL234IR",
								"James Doe",
								"BMW",
								"M5",
								"red");
	fac->listOfFlyweights();
	delete fac;
}
















