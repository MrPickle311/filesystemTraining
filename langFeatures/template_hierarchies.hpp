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

//Parametryzowanie dużych hierarchii klas zawierających wiele funkcji wirtualnych to zły pomysł
//przykład
template<typename Color_scheme, typename Canvas>
class Shape
{
//...
};
template<typename Color_scheme, typename Canvas>
class Circle :
		public Shape
{
//...
};

template<typename Color_scheme, typename Canvas>
class Triangle:
		public Shape
{
	//...
};

//generalnie lepiej nie przesadzać z parametryzacją ,w szczególności jeśli parametry są szczegółem implementacyjnym
//starać się unikać parametrów dotyczących kilku składowych , jeśli parametry są używane przez kilka funkcji składowych
//to lepiej uczynić te metody jako szablony

class Shape
{
	template<typename Color_scheme , typename Canvas>
	void configure(const Color_scheme&, const Canvas&);
	//...
};

//Dzięki szablonom można tworzyć≥ bezpieczne pod kątem typów interfejsy
//wywołania funkcji wirtualnych są kosztowne
//nie używaj void* gdyż nie jest bezpieczne pod kątem typów oraz zwykle trzeba implementować jakieś deskryptory
//typowe , co zwiększa narzut pamięciowy jak i wydajnościowy

template<typename N> //implementacja
struct Node_base
{ // nie wie o Val (dane użytkownika)
	N* left_child;
	N* right_child;
	Node_base();
	void add_left(N* p)
	{
		if (left_child==nullptr)
			left_child = p;
		else
		//...
	}
	//...
};

template<typename Val> //interfejs
struct Node:
		Node_base<Node<Val>>  // użycie klasy pochodnej jako części jej własnej bazy
{
	Val v;
	Node(Val vv);
	//...
};

using My_node = Node<double>;

//tutaj jednak użytkownik musi znać operacje na Node_base,gdyż sam musi na razie równoważyć drzewo
void user(const vector<double>& v)
{
	My_node root;
	int i = 0;

	for (auto x : v)
	{
	auto p = new My_node{x};
	if (i++%2) // wybór miejsca wstawiania
		root.add_left(p);
	else
		root.add_right(p);
	}
}


