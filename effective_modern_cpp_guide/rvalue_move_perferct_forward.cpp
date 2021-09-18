#include <string>
#include <type_traits>
#include <memory>
#include <vector>

void move_forward()
{
    //Thus, std::move casts its argument to an rvalue, and that’s all it does.

    //jak wygląda impl std::move
    template<typename T>
    decltype(auto) move(T&& param)
    {
        using ReturnType = remove_reference_t<T>&&;
        return static_cast<ReturnType>(param);
    }


    class Annotation {
    public:
        explicit Annotation(const std::string text)
        : value(std::move(text)) // ten kod nie przenosi , tylko kopiuje , bo text jest const
        {}
    // doesn't do what it seems to!
    private:
        std::string value;
    };

    // First, don’t declare objects
    // const if you want to be able to move from them. Move requests on const objects are
    // silently transformed into copy operations.

    // std::move performs an unconditional cast to an rvalue. In and of itself, it
    // doesn’t move anything.
    // std::forward casts its argument to an rvalue only if that argument is bound
    // to an rvalue.
    // Neither std::move nor std::forward do anything at runtime.

}

void rvalue_universal_ref()
{
    //Universal references arise in two contexts

    //function template

    template<typename T>
    void f(T&& param);

    //auto declarations,
    int var1{7};
    auto&& var2 = var1;

    //czyli jeśli mamy określony typ to mamy rvalue ref , jeśli nie to jest wtedy universal ref

    int&& y = 7;//rvalue 

    //Uwaga! && <- to jest zawsze jakiś rodzaj referencji !!! nie być przekazywaniem przez wartość

    //tutaj nie mamy już uniwersalnej referencji bo jest const
    template<typename T>
    void f(const T&& param);//rvalue no universal

    //jak przechwycić funkcję oraz jej parametry do lambdy

    auto timeFuncInvocation =
    [](auto&& func, auto&&... params)
    {
        // start timer;
        //invoke func
        std::forward<decltype(func)>(func)( std::forward<decltype(params)>(params)... );
        // stop timer and record elapsed time;
    };
}

void forward_to_universal_move_to_rvalue()
{
    class Widget {
    public:
        template<typename T>                 // universal reference
        void setName(T&& newName)           // compiles, but is
        { name = std::move(newName); }        // bad, bad, bad!
        
    private:
        class SomeDataStructure;
        std::string name;
        std::shared_ptr<SomeDataStructure> p;
    };

    Widget w;

    std::string name{"name"};

    w.setName(w);

    //i tutaj string jest przeniesiony!!!
    //jego wartość jest nieokreślona

    //lepiej już tutaj użyć std::forward , gdyż forward nie zawsze (conditionally) rzutuje do rvalue

    template<typename T>
    void setSignText(T&& text)
    {
        sign.setText(text);//use but dont modify
        auto now = std::chrono::system_clock::now();
        signHistory.add(now,
        std::forward<T>(text));// conditionally cast
                                // text to rvalue
    }

    template<typename T>     // by-value return
    Fraction            // universal reference param
    reduceAndCopy(T&& frac)
    {
        frac.reduce();

        return std::forward<T>(frac);   // move rvalue into return
                                        // value, copy lvalue
    }

    // RVO may be performed only if what’s being returned is a local object, !!!

    // Never apply std::move or std::forward to local objects if they would other‐
    // wise be eligible for the return value optimization.

    //jak i kiedy uzywać move?


    Widget makeWidget()
    {
        Widget w;
        return std::move(w);// źle , utrudniasz RVO robotę
    }

    Widget makeWidget()
    {
        Widget w;
        return w;//ok
    }

    //taką sytuację 
    Widget makeWidget(Widget w)
    {
        return w;
    }

    //kompilator widzi tak
    Widget makeWidget(Widget w)
    {
        return std::move(w);
    }
}

#include <set>

void avoid_overloading_and_universal_ref()
{
    std::multiset<std::string> names;
    
    //po prostu napisanie takiej funkcji
    template<typename T>
    void logAndAdd(T&& name)
    {
        auto now = std::chrono::system_clock::now();
        log(now, "logAndAdd");
        names.emplace(std::forward<T>(name));
    }

    std::string nameFromIdx(int idx);

    //i przeciążenie jej w taki sposób

    void logAndAdd(int idx)
    {
        auto now = std::chrono::system_clock::now();
        log(now, "logAndAdd");
        names.emplace(nameFromIdx(idx));
    }

    //po czym wowołanie jej z typem , który jest rzutowalny na int

    short nameid{4};

    //to sie nie skompiluje
    logAndAdd(nameid);

    //tak samo jest z konstruktorami przyjmującymi uniwersalne refy
    //a to dlatego ,że i tak po cichu będą tworzone move oraz copy ctory

    // Perfect-forwarding constructors are especially problematic, because they’re
    // typically better matches than copy constructors for non-const lvalues, and
    // they can hijack derived class calls to base class copy and move constructors.


}

class Widget{};
Widget widgetFactory(){ return {}; }

template<typename T>
void func(T&& param){}

void undersand_reference_collapsing()
{
    // lvalues are encoded as lvalue references, but rvalues are encoded as non-references

    Widget w;

    func(w); // call func with lvalue  T deduced
            // to be Widget&

    func(widgetFactory()); // call func with rvalue; T deduced
                            // to be Widget


    // If either reference is an lvalue reference, the result is an lvalue reference.
    // Otherwise (i.e., if both are rvalue references) the result is an rvalue refer‐
    // ence.

    auto&& w1 = w; //lvalue ref

    auto&& w2 = widgetFactory();

    //czyli jeśli są trzy znaczki &&& to są zbijane do jednego &
    //na tym polega reference collapsing

    //więcej znaczków & niż 3 nie da sie postawić

    // Reference collapsing occurs in four contexts: template instantiation, auto type
    // generation, creation and use of typedefs and alias declarations, and
    // decltype.

    // Universal references are rvalue references in contexts where type deduction
    // distinguishes lvalues from rvalues and where reference collapsing occurs.


}

void assume_that_move_operations_are_not_present()
{
    // small string optimization (SSO). With the
    // SSO, “small” strings (e.g., those with a capacity of no more than 15 characters)

    //przeniesienie wektora polega tylko na przenesieniu wskaźnika
    //do ciała danych
    //czas przeniesienia - stały

    std::vector<Widget> vw1;
    //put data
    auto vw2 = std::move(vw1);


    //moving and copying a std::array have linear-time computational complexity
    //because each element in the container must be copied or moved.
    std::array<Widget, 10000> aw1;

    auto aw2 = std::move(aw1);
}

void f(const std::vector<int>& v){}

template<typename... Ts>
void fwd(Ts&&... params)
{
    f(std::forward<Ts>(params)...);
}

//jakiś dekorator na funkcję
template<typename T>
T workOnVal(T param)
{
    //code
}

void perfect_forwarding_failure_cases()
{
    // Perfect forwarding fails when either of the following occurs:
    //Compilers are unable to deduce a type
    //Compilers deduce the “wrong” type

    fwd({3,2,5});//to się nie skompiluje
    //nie wiadomo na co skonwertować
    //kompilator ma zabronione takie coś
    //ale jakby się dało std::initializer_list to by działało

    f({2,3,2});

    auto il = { 1, 2, 3 };

    fwd(il);//to już zadziała bo znany jest typ

    //te dwie poniższe deklaracje
    //kompilator rozumie tak samo
    void f(int (*pf)(int));
    void f(int pf(int));

    //jeśli chcemy przekazać jakąś przeciążoną funkcję
    //do fwd to można zrobić jak poniżej

    //przeciążone funkcje
    int processVal(int value);
    int processVal(int value, int priority);

    using ProcessFuncType = int (*)(int);

    ProcessFuncType processValPtr = processVal;

    fwd(processValPtr);//działa

    //teraz szablon również zadziała
    fwd(static_cast<ProcessFuncType>(workOnVal)); // also fine

    //pola bitowe

    struct IPv4Header {
        std::uint32_t version:4,
                      L:4,
                      CP:6,
                      N:2,
                      talLength:16;
    };

    IPv4Header h;

    fwd(h.talLength);//to się nie skompiluje
    //tutaj jest wymagane podanie referencji
    //ale nie można pobrać adresu pola bitowego

    //istnieje reguła
    // “A non-const reference shall not be bound to a bit-field.”

    // by to zadziałało trzeba przecastować wartość pola bitowego i ją skopiować

    // copy bitfield value; see Item 6 for info on init. form
    auto length = static_cast<std::uint16_t>(h.talLength);

    fwd(length); // forward the copy
}

int main()
{
    //184
}