#pragma once

//trochę ulepszona struktura pozwalająca wybrać algorytm balansujacy jako parametr szablonu

//Optymalizacja pustej bazy
//Jeśli klasa bazowa nie zawiera niestatycznych danych składowych ,to nie zostanie dla niej alokowana pamięć w obiekcie klasy pochodnej
//używanie void* narzuca koszt wykonawczy

struct Red_black_balance
{
	//...
};

template<typename N, typename Balance>
struct Node_base:
		public Balance
{
	N* left_child;
	N* right_child;
	Node_base();
	void insert(N& n)
	{
		//...
		if (this->compare(n,left_child))//... jakieś działania...
		else
		//... jakieś inne działania...
		// używa compare() z Balance
	}
};

template<typename Val, typename Balance>
struct Search_node:
		public Node_base<Search_node<Val, Balance>, Balance>
{
	Val val; // dane użytkownika
	Search_node(Val v): val(v) {}
};

template<typename T>
using Rb_node = Search_node<T,Red_black_balance>; // alias typu dla drzew czerwono-czarnych
using My_node = Rb_node<double>;

void user(const vector<double>& v)
{
	Rb_node<double> my_root; // drzewo czerwono-czarne wartości typu double
	for (auto x : v)
	my_root.insert(*new My_node{x});
}

//alternatywna implementacja pozwalająca na uczynienie mechanizmu równoważącego niejawnym argumentem w postaci typu powiązanego
//wykorzystanie techniki CRTP (curiously recurring template pattern)
template<typename N>
struct Node_base2 :
		public N::balance_type // (1)
{
	//...
};


template<typename Val,typename Balance>
struct Search_node2:
		public Node_base2<Search_node2<Val,Balance>> //udostępnienie usingów
{
	using balance_type = Balance; // to ląduje do (1)
};

//lineryzacja hierarchii klas

namespace ipr //interfejsy
{
	class Visitor{};
	using Category_code = unsigned int;
	class Type{};
	class Unit_location{};
	class Source_location{};
	class Annotation{};
	template<typename T>
	class Sequence{};
	class Linkage{};
	class Name{};
	class Region{};
	class Named_map{};
	class Substitution{};
	class var_cat{};

	struct Node
	{
		const int node_id;
		const Category_code category;
		virtual void accept(Visitor&) const = 0; // punkt zaczepienia dla klas wizytujących
	protected:
		Node(Category_code); //jest bazą więc konstruktor jest chroniony
	};

	struct Expr :
			public Node
	{
		virtual const Type& type() const = 0;
	protected:
		Expr(Category_code c) : Node(c) { }
	};

	struct Stmt:
			public Expr
	{
		virtual const Unit_location& unit_location() const = 0;
		 // wiersz w pliku
		virtual const Source_location& source_location() const = 0; // plik
		virtual const Sequence<Annotation>& annotation() const = 0;
	protected:
		Stmt(Category_code c) : Expr(c) { }
	};

	struct Decl:
			public Stmt
	{
		enum Specifier { /* klasa przechowująca, wirtualność, kontrola dostępu itd.*/};
		virtual Specifier specifiers() const = 0;
		virtual const Linkage& lang_linkage() const = 0;
		virtual const Name& name() const = 0;
		virtual const Region& home_region() const = 0;
		virtual const Region& lexical_region() const = 0;
		virtual bool has_initializer() const = 0;
		virtual const Expr& initializer() const = 0;
		//...
	protected:
		Decl(Category_code c) : Stmt(c) { }
	};

	template<Category_code Cat, typename T = Expr> //pomoc notacyjna
	struct Category:
			public T
	{
	protected:
		Category() : T(Cat) { }
	};

	struct Var :
			public Category<var_cat, Decl>
	{
	};
}

namespace impl  //implementacje
{
	class var_cat{};
	template<typename  T>
	class ref_sequence{};
	template<typename  T>
	class basic_decl_data{};
	template<typename  T>
	class val_sequence{};


	template<typename T>// T jest klasą z ipr::Node
	struct Node:
			public T
	{
		using Interface = T; // czyni typ argumentu szablonu dostępnym dla użytkowników
		void accept(ipr::Visitor& v) const override { v.visit(*this); }
	};

	template<typename Interface> //Interface jest jakąkolwiek klasą pochodną ipr::Expr
	struct Expr:
			public impl::Node<Interface>
	{
		const ipr::Type* constraint; // ograniczenie jest typem wyrażenia
		Expr() : constraint(0) { }
		const ipr::Type& type() const override { return *util::check(constraint); }
	};

	template<typename S>
	struct Stmt:
			public S
	{
		ipr::Unit_location unit_locus; // logiczna pozycja w jednostce translacji
		ipr::Source_location src_locus; // plik źródłowy, wiersz i kolumna
		ref_sequence<ipr::Annotation> notes;
		const ipr::Unit_location& unit_location() const override { return unit_locus; }
		const ipr::Source_location& source_location() const override { return src_locus; }
		const ipr::Sequence<ipr::Annotation>& annotation() const override { return notes; }
	};

	template<typename D>
	struct Decl:
			public impl::Stmt<impl::Expr<D>> // używa implementacji Stmt i Expr
	{
		basic_decl_data<D> decl_data;
		ipr::Named_map* pat;
		val_sequence<ipr::Substitution> args;
		Decl() : decl_data(0), pat(0) { }
		const ipr::Sequence<ipr::Substitution>& substitutions() const { return args; }
		const ipr::Named_map& generating_map() const override { return *util::check(pat); }
		const ipr::Linkage& lang_linkage() const override;
		const ipr::Region& home_region() const override;
	};

	struct Var :
			public impl::Decl<ipr::Var>
	{
		const ipr::Expr* init;
		const ipr::Region* lexreg;
		Var();
		bool has_initializer() const override;
		const ipr::Expr& initializer() const override;
		const ipr::Region& lexical_region() const override;
	};

}
