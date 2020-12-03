#pragma once

#include <iostream>
#include <string>

//Nadawanie kodowi wysoki poziom elastycznosci
//obecne metody kreacyjne i kreatorzy

///zastosowanie gdy:
/// - nie wiem z gory jakie typy pojawia sie w programie i jakie beda miedzy nimi zaleznosci
/// - chce pozwolic uzytkownikom mojej biblioteki rozbudowywac jej wewnetrzne komponenty
/// - oszczedzanie zasobow systemowych poprzez ponowne wykorzystanie juz istniejacych obiektow
///

//ponowne wykorzystanie obiektow
//1. stworzenie magazynu pamietajacego stworzone obiekty
//2. redystrybucja wolnych obiektow na zadanie kodu
//3. w razie braku wolnych obiektow, tworzenie nowych
//4. skalowanie ilosci obiektow ,by nie zapchac pamieci
//5. potrzebna jest metoda ,ktora potarfi zarowno tworzyc nowe obiekty
//jak i zwracac istniejace ,a to zadanie idealnie pasuje
//dla tego wzorca projetkowego

// oddzielenie kodu konstruujacego produkty od kodu ich uzywajacego
// tworzenie ,,twardych" wariantow klas oraz specjalnych kreatorow dla nich

//IMPLEMENTACJA
//1. wszystkie produkty maja ten sam interfejs
//2. Dodanie pustej metody wytworczej do klasy kreacyjnej.
//zwracany typ powinien byc zgodny z interfejsem wspolnym dla produktow
//3.

class Product
{
public:
	virtual ~Product() {}
	virtual std::string operation() const = 0;
};

//Interfejs produktu deklaruje operacje ,ktore inne konkretne
//produkty musza implementowac


//Konkretne produkty z roznorodna implementacja ,ale z interfejsem
//branym z Product

class ConcreteProduct1 :
		public Product
{
	// Product interface
public:
	inline virtual std::string operation() const override
	{
		return {"Product 1 operation"};
	}
};


class ConcreteProduct2:
		public Product
{
	// Product interface
public:
	inline virtual std::string operation() const override
	{
		return {"Product 2 operation"};
	}
};

//Teraz czas na kreatora, klasa kreatora deklaruje metode wytworcza
//ktora jest odpowiedzialna za zwrocenie jakiegos obiektu klasy.
//Jest to METODA wytworcza, abstrakcja , tutaj odbywa sie sterowanie
//Klasy pochodne zapewniaja skonkretyzowane implementacje tej metody

class Creator
{
	//Kreator moze zapewniac domyslna implementacje metody wytworczej

public:
	virtual ~Creator() {}
	virtual Product* factoryMethod() const = 0;

	//Kreator nie jest sam w sobie odpowiedzialny za tworzenie
	//obiektow. Zwykle zawiera w sobie troche biznesowej logiki
	//Klasy pochodne moga niejawnie zmienic logike biznesowa
	//przeciazajac wirtualne funkcje ,ktore niejawnie
	//zwracaja inny typ

	std::string someOperation() const
	{
		//uwaga! nagie wskazniki !!!
		//fabryka wytwarza Product
		Product* product {this->factoryMethod()};
		//uzycie produktu
		std::string res {"Creator operation text " + product->operation()};
		delete product;
		return res;
	}
};

//konkretni kreatorzy

class ConcreteCreator1 :
		public Creator
{
	// Creator interface
public:
	virtual Product* factoryMethod() const override
	{
		return  new ConcreteProduct1{};
	}
};

class ConcreteCreator2:
		public Creator
{
	// Creator interface
public:
	virtual Product* factoryMethod() const override
	{
		return new ConcreteProduct2{};
	}
};

void clientCode(const Creator& creator)
{
	//...
	std::cout << "Client operations " << creator.someOperation() << std::endl;
	//...
}

//aplikacja zaleznie od srodowiska ,czy innych warunkow
//wybiera innego kreatora

void fm_main()
{
	std::cout << "Maybe creator 1?" << std::endl;
	Creator* creator1 {new ConcreteCreator1{}};
	clientCode(*creator1);
	std::cout << '\n';
	std::cout << "Maybe creator 2 ?" << std::endl;
	Creator* creator2 {new ConcreteCreator2{}};
	clientCode(*creator2);

	delete creator1;
	delete creator2;
}
