#pragma once

/*Kiedy wybrać szablony , a kiedy hierarchie klas?
 * Przewaga szablonów:
 * 1.Jeżeli typy w interfejsie do generowanych lub pochodnych klas muszą się różnić
 * 2.Jeśli implementacje wygenerowanych lub pochodnych klas różnią się tylko parametrem
 * lub w kilku specjalnych przypadkach
 * 3.Jeśli niemile widziane jest użycie wolnej pamięci
 * 4.Jeśli stawiam jako pierwszy cel wydajność
 * Przewaga hierarchii klas:
 * 1.Gdy rzeczywiste typy używanych obiektów nie są znane w czasie kompilacji
 * 2.Jeśli wygenerowane lub pochodne typy musi łączyć relacja hierarchiczna. (potrzeba
 * konwersji)
 */

/*Dodatkowe wskazówki
 * 1.Przedkładać kontenery nad wbudowane tablice
 * 2.Podejrzliwie traktur interfejsy void f(T* p,int c) ,gdy T to klasa bazowa
 */

//Konwersje szablonów

//przypuśćmy ,że jest sobie taka klasa

class Shape {};
class Circle:
		public Shape
{};

template<typename T>
class Ptr { // wskaźnik do T
	T* p;
public:
	Ptr(T*);
	Ptr(const Ptr&);
 // konstruktor kopiujący
	template<typename T2>
	explicit operator Ptr<T2>() // konwersja Ptr<T> na Ptr<T2>
	{
		/* Instrukcja return da się skompilować tylko wtedy, gdy p
		 * (będący obiektem typu T*) będzie mógł być argumentem konstruktora Ptr<T2>(T2*)
		 */
		return Ptr<T2>{p};
	}
	//...
};


//załóżmy przykładowe użycie :

void f(Ptr<Circle> pc)
{
	Ptr<Shape> ps {pc}; // powinno działać
	Ptr<Circle> pc2 {ps}; // powinno być błędem
}

