#pragma once

/*Zastosowanie:
 * 1.Gdy kolekcja z którą masz do czynienia posiada skomplikowaną strukturę,
 * ale zależy ci na ukryciu jej przed klientem
 * 2.W celu redukcji duplikowania kodu przeglądania elementów zbiorów na
 * przestrzeni całego programu
 * 3.Gdy chcesz, aby twój kod był w stanie przeglądać elementy różnych
 * struktur danych, lub gdy nie znasz z góry szczegółów ich struktury.
 */

/*Implementacja
 * 1.Deklaracja interfejsu iteraotora
 * 2.Deklaracja interfejsu kolekcji i metody zwracającą iteratory
 * 3.Implementacja konkretnych klas kolekcji i iteratorów. Obiekt kolekcji musi
 * przekazać samego siebie do konstruktora iteratora,w celu stworzenia powiązania
 */

/*Zalety
 * 1.Spełnia Single Responsibility Principle
 * 2.Spełnia Open/Closed Principle
 * 3.Można przeglądać kolekcję wieloma iteratorami równolegle
 * 4.Możliwość sterowania iteracją
 */

/*Wady:
 * 1.Wzorzec bez sensu dla prostych kolekcji
 * 2.Iteratory są wygodne kosztem wydajności
 */

/*Powiązania:
 * 1.Za pomocą iteratorów mogą służyć do przemieszczania się po drzewie Kompozytowym
 * 2.Metoda wytwórcza może sprawić ,że konretne kolekcje mogą zwracać konkretne iteratory
 * 3.Można zastosować Pamiątkę wraz z Iteratorem by zapisać bieżący stan iteracji,co
 * pozwoli w razie czego do niego wrócić
 * 4.Połączenie Wizytatora z Iteratorem może służyć sekwencyjnemu przeglądaniu elementów
 * złożonej struktury danych i wykonaniu na nich jakiegoś działania ,nawet jeśli te elementy
 * są obiektami różnych klas
 */


#include <iostream>
#include <string>
#include <vector>

//taki sobie przykład iteratora , wiadomo jak działa
template <typename T, typename U>
class Iterator {
 public:
  typedef typename std::vector<T>::iterator iter_type;
  Iterator(U *p_data, bool reverse = false) : m_p_data_(p_data)
  {
	m_it_ = m_p_data_->m_data_.begin();
  }

  void first()
  {
	m_it_ = m_p_data_->m_data_.begin();
  }

  void next()
  {
	m_it_++;
  }

  bool isDone()
  {
	return (m_it_ == m_p_data_->m_data_.end());
  }

  iter_type current()
  {
	return m_it_;
  }

 private:
  U *m_p_data_;
  iter_type m_it_;
};

template <class T>
class Container {
  friend class Iterator<T, Container>;

 public:
  void add(T a)
  {
	m_data_.push_back(a);
  }

  Iterator<T, Container> *CreateIterator()
  {
	return new Iterator<T, Container>(this);
  }

 private:
  std::vector<T> m_data_;
};

class Data {
 public:
  Data(int a = 0) : m_data_(a) {}

  void set_data(int a)
  {
	m_data_ = a;
  }

  int data()
  {
	return m_data_;
  }

 private:
  int m_data_;
};

void clientCode() {
  std::cout << "________________Iterator with int______________________________________" << std::endl;
  Container<int> cont;

  for (int i = 0; i < 10; i++)
  {
	cont.add(i);
  }

  Iterator<int, Container<int>> *it = cont.CreateIterator();
  for (it->first(); !it->isDone(); it->next())
  {
	std::cout << *it->current() << std::endl;
  }

  Container<Data> cont2;
  Data a(100), b(1000), c(10000);
  cont2.add(a);
  cont2.add(b);
  cont2.add(c);

  std::cout << "________________Iterator with custom Class______________________________" << std::endl;
  Iterator<Data, Container<Data>> *it2 = cont2.CreateIterator();
  for (it2->first(); !it2->isDone(); it2->next())
	std::cout << it2->current()->data() << std::endl;

  delete it;
  delete it2;
}

int main() {
  clientCode();
  return 0;
}
