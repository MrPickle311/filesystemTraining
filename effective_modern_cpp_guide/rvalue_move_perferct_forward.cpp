#include <string>
#include <type_traits>
#include <memory>

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

int main()
{
    //177
}