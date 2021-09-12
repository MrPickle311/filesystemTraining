#include <iostream>
#include <vector>
#include <type_traits>

class Widget 
{
public:
    Widget(int i, bool b);
    Widget(int i, double d);
    Widget(std::initializer_list<long double> il);

    operator float() const;
};

class Widget1 
{
public:
    Widget(int i, bool b);
    Widget(int i, double d);
    Widget(std::initializer_list<bool> il);

    operator float() const;
};


class Widget2 
{
public:
    Widget2(int i, bool b);
    Widget2(int i, double d);
    Widget2(std::initializer_list<std::string> il);

    operator float() const;
};

void init()
{
    Widget w1(10, true); // uses parens and, as before,
    // calls first ctor
    
    Widget w2{10, true}; // uses braces, but now calls
    // std::initializer_list ctor
    // (10 and true convert to long double)

    Widget w5(w2); // uses parens, calls copy ctor

    Widget w6{w2}; 
    // uses braces, calls
    // std::initializer_list ctor
    // (w4 converts to float, and float
    // converts to long double)

    // {} - wymusza użycie konstruktora z std::initializer_list

    Widget w4({}); // calls std::initializer_list ctor with empty list
    
    //narrowing conversion!!!
    Widget1 w{10, 5.0};

    //there is no way to convert ints and bools to std::string
    Widget2 w1(10, true); // uses parens, still calls first ctor
    Widget2 w2{10, true}; // uses braces, now calls first ctor
    Widget2 w3(10, 5.0); // uses parens, still calls second ctor
    Widget2 w4{10, 5.0}; // uses braces, now calls second ct


    Widget w4({}); // calls std::initializer_list ctor with empty list

    //example

    std::vector<int> v1(10, 20); //
    // use non-std::initializer_list
    // ctor: create 10-element
    // std::vector, all elements have
    // value of 20

    std::vector<int> v2{10, 20}; 
    // use std::initializer_list ctor:
    // create 2-element std::vector,
    // element values are 10 and 2    
}   

int* abc();

void pointers()
{
    //nullptr jest typu nullptr_t , który się konwertuje na każdy typ wskaźnika ,ale nie na liczbę

    int a = nullptr;
    int* a = nullptr;

    int b = NULL;
    int* b = NULL;

    auto bb {abc()};//better

    //Avoid overloading on integral and pointer types.
}

void typedef_alias()
{
    typedef void (*FP)(int, const std::string&);
    // typedef
    // same meaning as above

    using FP = void (*)(int, const std::string&);
    // alias
    // declaration

    // alias declarations may be
    // templatized (in which case they’re called alias templates), while typedefs cannot.

    //różnica

    template<typename T>
    using MyAllocList = std::list<T, MyAlloc<T>>; // MyAllocList<T>
                                                  // is synonym for
                                                  // std::list<T, MyAlloc<T>>

    MyAllocList<Widget> lw; // client code

    template<typename T> // MyAllocList<T>::type
    struct MyAllocList {    // is synonym for // std::list<T, MyAllocList<T>>
        typedef std::list<T, MyAlloc<T>> type; 
    };

    MyAllocList<Widget>::type lw;
    // client code

    // Alias templates avoid the “::type” suffix and, in templates, the “typename”
    // prefix often required to refer to typedefs.
    // • C++14 offers alias templates for all the C++11 type traits transformations.
}

void enums()
{
    enum Color { black, white, red }; // te enumy lecą do globalnej przestrzeni nazw

    auto white {4};//błont

    enum class Color { black, white, red };// ale enum class nie 

    auto white {4};//lepiej

    // dodatkowo enum classy nie pozwalają na pośrednią konwersję na liczby

    enum class Status: std::uint32_t;

    //ale czasami zwykłe enumy się przydają
    
    using UserInfo =
        std::tuple<std::string,
                   std::string,
                   std::size_t>;
    
    enum UserInfoFields { uiName, uiEmail, uiReputation };

    UserInfo uInfo;

    auto val = std::get<uiEmail>(uInfo);// gdyby UserInfoFields był enum classem , to trzeba ,by było użyc static_cast<>

    // Scoped enums may always be forward-declared. Unscoped enums may be
    // forward-declared only if their declaration specifies an underlying type.
}

void delete_features()
{
    //jeśli mamy sobie taką funkcje
    template<typename T>
    void processPointer(T* ptr);

    //to tak możemy wyłączyć jej niektóre specjalizacje
    template<>
    void processPointer<void>(void*) = delete;
    
    template<>
    void processPointer<char>(char*) = delete;

    //a tak można z szablonami klas
    class Widget {
    public:
        template<typename T>
        void processPointer(T* ptr)
        {  }
    };

    template<>
    void Widget::processPointer<void>(void*) = delete;
}

void inheritance_override()
{
    class Widget 
    {
        std::vector<double> values_;
    public:
        std::vector<double>& doWork() & // this version of doWork applies
        {                               // only when *this is an lvalue
            return values;
        }               

        std::vector<double> doWork() &&; // this version of doWork applies
        {                                 // only when *this is an rvalue
            return std::move(values);//IMPROVE PERFORMANCE
        }


    };

    Widget makeWidget();

    Widget w;

    w.doWork(); // calls Widget::doWork for lvalues
    makeWidget().doWork(); // calls Widget::doWork for rvalues MOVE

    // jeśli mam poniższą sytuację , to kompilator może to puścić
    
    class Base {
    public:
        virtual void mf1() const;
        virtual void mf2(int x);
        virtual void mf3() &;
        void mf4() const;
    };

    class Derived: public Base {
    public:
        virtual void mf1();
        virtual void mf2(unsigned int x);
        virtual void mf3() &&;
        void mf4() const;
    };

    //ale jak dam override , to będę mieć błąd kompilacji , co jest ok
    class Derived: public Base {
    public:
        virtual void mf1() const override;
        virtual void mf2(int x) override;
        virtual void mf3() & override;
        void mf4() const override;
    };


}

void except()
{
    int function(int params) noexcept; // most optimizable
    int function(int params) throw(); // less optimizable
    int function(int params); // less optimizable

    template <class T, size_t N>
    void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)));
    
    template <class T1, class T2>
    struct pair 
    {
        void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
                           noexcept(swap(second, p.second)));
    };

    // These functions are conditionally noexcept: whether they are noexcept depends on
    // whether the expressions inside the noexcept clauses are noexcept

    // noexcept is part of a function’s interface, and that means that callers may
    // depend on it.
    // • noexcept functions are more optimizable than non-noexcept functions.
    // • noexcept is particularly valuable for the move operations, swap, memory
    // deallocation functions, and destructors.
    // • Most functions are exception-neutral rather than noexcept.
}

int main()
{
    //97
}