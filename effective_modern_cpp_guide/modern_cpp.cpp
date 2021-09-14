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

void constexpr_use_whenever_is_possible()
{
    //wartości obiektów constexpr są znane w trakcie kompilacji
    // They may be placed in read-only
    // memory, for example, and, especially for developers of embedded systems, this can
    // be a feature of considerable importance

    constexpr auto arraySize2 = 10;

    int sz;

    constexpr auto arraySize1 = sz; // error! not constant

    // const doesn’t offer the same guarantee as constexpr, because const
    // objects need not be initialized with values known during compilation:

    // Simply put, all constexpr objects are const, but not all const objects are constexpr.

    // constexpr functions produce compile-time constants when they
    // are called with compile-time constants
    // If they’re called with values not known until
    // runtime, they produce runtime values.
    //they can be called in runtine and compile time

    //In C++11, constexpr functions may contain no more than a single executable state‐
    //ment: a return.

    constexpr int pow(int base, int exp) noexcept
    {
        return (exp == 0 ? 1 : base * pow(base, exp - 1));
    }

    // In C++11, all
    // built-in types except void qualify, but user-defined types may be literal, too, because
    // constructors and other member functions may be constexpr:

    //in c++11 setters cannnot be constexpr bcs they modify Point's values
    class Point 
    {
    public:
        constexpr Point(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
        {}
        constexpr double xValue() const noexcept { return x; }
        constexpr double yValue() const noexcept { return y; }
        void setX(double newX) noexcept { x = newX; }
        void setY(double newY) noexcept { y = newY; }
    private:
        double x, y;
    };

    //but in c++14 they can be 

    class Point1 
    {
    public:
        constexpr Point1(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
        {}
        constexpr double xValue() const noexcept { return x; }
        constexpr double yValue() const noexcept { return y; }
        constexpr void setX(double newX) noexcept { x = newX; }
        constexpr void setY(double newY) noexcept { y = newY; }
    private:
        double x, y;
    };

    //That makes it possible to write functions like this:
    constexpr Point reflection(const Point& p) noexcept
    {
        Point result;
        // create non-const Point
        result.setX(-p.xValue());
        result.setY(-p.yValue()); // set its x and y values
        return result; // return copy of it
    }

    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);
    constexpr auto mid = midpoint(p1, p2);

    constexpr auto reflectedMid = reflection(mid);
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

void thread_safety()
{
    // For a single variable or memory location requiring synchroni‐
    // zation, use of a std::atomic is adequate, but once you get to two or more variables
    // or memory locations that require manipulation as a unit, you should reach for a
    // mutex.

    // Make const member functions thread safe unless you’re certain they’ll never
    // be used in a concurrent context.
}

int main()
{
    //109
}