#pragma once

#include <memory>

//Pierwsza wersja

using namespace std;

class BBwidget //jakaś klasa systemowa zapewniająca nam środowisko graficzne
{
	//... skomplikowane rzeczy
};

class Ival_box:
		public BBwidget
{
protected:
	int val;
	int low, high;
	bool changed {false}; // zmieniane przez użytkownika za pomocą funkcji set_value()
public:
	Ival_box(int ll, int hh) :val{ll}, low{ll}, high{hh} { }
	virtual int get_value() { changed = false; return val; } // dla aplikacji
	virtual void set_value(int i) { changed = true; val = i; }  // dla użytkownika
	virtual void reset_value(int i) { changed = false; val = i; } // dla aplikacji
	virtual void prompt() { }
	virtual bool was_changed() const { return changed; }
	virtual ~Ival_box() {};
};

class Ival_slider : public Ival_box
{
private:
	//... graficzna reprezentacja suwaka itp.
public:
	Ival_slider(int, int);
	int get_value() override; // pobiera wartość od użytkownika i zapisuje ją w val
	void prompt() override;
};

class Ival_dial:
		public Ival_box
{ /*...*/};

class Flashing_ival_slider:
		public Ival_slider
{ /*...*/};

class Popup_ival_slider:
		public Ival_slider
{ /*...*/};

//wygląd hierarchii klas

/*					BBwidget
 *						^
 *					    |
 *					  Ival_box
 *				    ^		 ^
 *				   /          \
 *			 Ival_slider        Ival_dial
 *			^         ^				    ^
 *		   /          \                  \
 *Popup_ival_slider Flashing_ival_slider  Ival_dial
 */

//problemem jest to dziedziczymy implementację po BBwidget ,ale jest możliwość ,że
//chcielibyśmy dziedziczyć po klasie innej firmy, w takim razie wszystko trzeba pisać
//od nowa
//Jest tutaj duża wrażliwość na zmiany i aktualizacje BBwidget

//klas z tej hierarchii można używać następująco
void interact(Ival_box* pb)
{
	int old_val = pb->get_value();
	pb->prompt(); // wyświetla prośbę do użytkownika
	//...
	int i = pb->get_value();
	if (i != old_val) {
	//... nowa wartość; jakieś działania...
	}
	else {
	//... jakieś inne działania...
	}
}

void some_fct()
{
	unique_ptr<Ival_box> p1 {new Ival_slider{0,5}};
	interact(p1.get());
	unique_ptr<Ival_box> p2 {new Ival_dial{1,12}};
	interact(p2.get());
}
///
