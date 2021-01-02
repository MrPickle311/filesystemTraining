#pragma once
class BBwidget
{
	//...
};


//No dobra ,jeszcze raz ,oto pewna założenia:
/*
 * 1.System interfejsu użytkownika powinien być szczegółem implementacyjnym ukrytym
 *przed użytkownikami, których jego istnienie nie interesuje.
 * 2.Klasa Ival_box nie powinna zawierać jakichkolwiek danych.
 * 3.Zmiana w systemie interfejsu użytkownika nie powinna wymagać rekompilacji kodu,
 *w którym wykorzystywana jest rodzina klas Ival_box.
 * 4.W programie powinny móc współistnieć klasy Ival_box dla różnych systemów inter-
 *fejsów użytkownika.
 */

//czysty interfejs
class Ival_box
{
public:
	virtual int get_value() = 0;
	virtual void set_value(int i) = 0;
	virtual void reset_value(int i) = 0;
	virtual void prompt() = 0;
	virtual bool was_changed() const = 0;
	virtual ~Ival_box() { } //wirtualny konstruktor istnieje w celu zapewnienia poprawności
							//kasowania danych w klasach pochodnych
};


class Ival_slider :
		public Ival_box,	//dostarcza interfejs więc jest publiczna
		protected BBwidget	//dostarcza implementację ,więc jest chroniona, nie jest
							//prywatna ,gdyż dostarcza implementację również do klas
							//pochodnych Ival_slider
{
public:
	Ival_slider(int,int);
	~Ival_slider() override; //lepiej używać ovveride ,by się nie pomylić
	int get_value() override;
	void set_value(int i) override;
	//...
protected:
	//... funkcje przesłaniające funkcje wirtualne klasy BBwidget,
	// np. BBwidget::draw(), BBwidget::mouse1hit() itd.
private:
	//... dane potrzebne suwakowi...
};

class Ival_dial:
	public Ival_box,
		protected BBwidget
{ /*...*/};

class Flashing_ival_slider:
	public Ival_slider
{ /*...*/};

class Popup_ival_slider:
	public Ival_slider
{ /*...*/};


//struktura

/*
 * BBwidget							Ival_box							BBwidget
 *		^							^      ^							^
 *		 *						  /			\						   *
 *        *					   /			  \						  *
 *		   *				/					\					*
 *			*			  /						 \				   *
 *			  Ival_slider								Ival_dial
 *			  ^			^
 *			/			 \
 *Popup_ival_slider		  Flashing_ival_slider
 */

// *** -> dziedziczenie chronione


//ten przykład nie jest najlepszy ,ale by był lepszy należy przekształcić go używając
//wzorców projektowych oraz trzymając się dwóch zasad:
/*
 * 1.Używaj klas abstrakcyjnych w celu umożliwienia dziedziczenia interfejsu
 * 2.Używaj klas bazowych z implementacjami funkcji wirtualnych w celu wykorzystania
 * dziedziczenia implementacji
*/
