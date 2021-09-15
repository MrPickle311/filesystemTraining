#include <memory>
#include <vector>

class Investment
{
public:
    virtual ~Investment(); // must to have!!!!
};

class Stock:
public Investment {  };

class Bond:
public Investment {  };


void makeLogEntry(Investment*);

void delInvmt2(Investment* pInvestment)
{
    makeLogEntry(pInvestment);
    delete pInvestment;
}

void unique_ptr()
{
    // It’s reasonable to assume that, by default, std::unique_ptrs are the
    // same size as raw pointers,


    //defaultowy deleter
    // The custom deleter takes a parameter of type Investment* !!!
    // evaluates derived classes dtors!!!
    auto delInvmt = [](Investment* pInvestment)
    {
        makeLogEntry(pInvestment);
        delete pInvestment;
    };

    //funkcja fabryki dla klasy Investment

    template<typename... Ts>
    std::unique_ptr<Investment, decltype(delInvmt)> makeInvestment(Ts&&... params)
    {
        std::unique_ptr<Investment, decltype(delInvmt)> ptr{nullptr, delInvmt};

        ptr.reset(new Stock(std::forward<Ts>(params)...));

        return ptr;
    }

    //ale jeśli zamiast lambdy damy funkcję jako deleter , 
    //to rozmiar std::unique_ptr się zwiększy o rozmiar wskaźnika na tę funkcje!!!

    template<typename... Ts>                            // has size of
    std::unique_ptr<Investment, decltype(delInvmt1)>// Investment*
    makeInvestment(Ts&&... args); // return type


    template<typename... Ts>                                    
    std::unique_ptr<Investment, void (*)(Investment*)> //ptr size + function ptr size
    makeInvestment(Ts&&... params);

    //unique ptr'y mają operator konwersji do shared_ptr
}

class Widget{};

void shared_ptr()
{
    //std::shared_ptrs are twice the size of a raw pointer,
    //ptr obj and reference count obj ptr

    //Moving std::shared_ptrs is therefore faster than copying them: copying requires
    // incrementing the reference count, but moving doesn’t.

    auto customDeleter1 = [](Widget *pw) {};
    auto customDeleter2 = [](Widget *pw) {};

    //deletery mogą być różnych typów , każdy ptr do tego samego obiektu
    //może mieć różny deleter

    std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
    std::shared_ptr<Widget> pw2(new Widget, customDeleter2);
    
    // deletery nie zwiększają rozmiaru shared_ptr

    //see shared_ptr_working.png in this folder

    // * std::make_shared always creates a control block.
    // * A control block is created when a std::shared_ptr is constructed from a
    // unique-ownership or weak pointer
    // *When a std::shared_ptr constructor is called with a raw pointer, it creates a control block.


    class XYZ
    {
    public:
        std::vector<std::shared_ptr<XYZ>> wdigets_;

        void process()
        { 
            wdigets_.emplace_back(this); //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!
        }
    };

    auto w2{std::make_shared<XYZ>()}; //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!

    w2->process(); //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!

    // W powyższym kodzie będzie undefined behaviour!!! dwa addzielne bloki

    //ale jeśli chcemy mieć możliwość takiego zachowania , to musimy użyć std::enable_shared_from_this

    // spełnia on zasadę -> The Curiously Recurring Template Pattern  CRTP

    class XYZ : public std::enable_shared_from_this<XYZ>
    {
    public:
        std::vector<std::shared_ptr<Widget>> wdigets_;

        void process()
        { 
            wdigets_.emplace_back(shared_from_this()); //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!
        }
    };

    // std::enable_shared_from_this defines a member function that creates a
    // std::shared_ptr to the current object, but it does it without duplicating control
    // blocks.

    auto w2{std::make_shared<XYZ>()}; //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!

    w2->process(); // a tu nie 

    //i ten kod jest ok

    //ale ten obiekt potrzbuje tego ,by ten obiekt był już opakowany std::shared_ptr
    //nagi wskaźnik do XYZ się wyłoży
    // For that to be the case, there
    // must be an existing std::shared_ptr that points to the current object. If no such std::shared_ptr
    // exists, behavior is undefined, although shared_from_this typically throws an exception.

    //ale wystarczy schować konstruktor i stworzyć metodę fabryki

    class XYZ : public std::enable_shared_from_this<XYZ>
    {
    public:
        std::vector<std::shared_ptr<Widget>> wdigets_;

        template<typename... Ts>
        static std::shared_ptr<Widget> create(Ts&&... params)
        {
            // factory function that perfect-forwards args
            // to a private ctor
        }

        void process()
        { 
            wdigets_.emplace_back(shared_from_this()); //TUTAJ SIĘ TWORZY NOWY CONTROL BLOCK!!!
        }
    private:
        //ctors
    };
}

class WidgetID{};

#include <unordered_map>

void weak_ptr()
{
    //są używane do read-only operations
    //mogą sprawdzać stan oraz mogą być przekształcone w shared_ptr

    //observer design pattern
    std::shared_ptr < const Widget> fastLoadWidget1(WidgetID id)
    {
        static std::unordered_map<WidgetID,std::weak_ptr<const Widget>> cache;

        auto objPtr = cache[id].lock(); // objPtr is std::shared_ptr
                                        // to cached object (or null
                                        // if object's not in cache)
        
        if (!objPtr) 
        {
            objPtr = loadWidget(id);     // load it
            cache[id] = objPtr;          // cache it
        }

        return objPtr; // if not in cache,
    }

    // std::weak_ptr objects are the same size as std::shared_ptr

    // Potential use cases for std::weak_ptr include caching, observer lists, and the
    // prevention of std::shared_ptr cycles.


}

int computePriority();

void processWidget(std::shared_ptr<Widget> , int);

void prefer_usage_make_shared_unique()
{
    processWidget(std::shared_ptr<Widget>(new Widget), computePriority()); // potential
                                                                            // resource
                                                                            // leak
    // muszą zostać wykonane 3 operacje:
    // 1. Perform “new Widget”.
    // 2. Execute computePriority.
    // 3. Run std::shared_ptr constructor.

    // Compilers are not required to generate code that executes them in this order

    // If such code is generated and, at runtime, computePriority produces an exception,
    // the dynamically allocated Widget from Step 1 will be leaked, because it will never be
    // stored in the std::shared_ptr that’s supposed to start managing it in Step 3.

    //better
    processWidget(std::make_shared<Widget>(), computePriority());

    // Using std::make_shared allows compilers to generate smaller, faster
    // code that employs leaner data structures

    // std::make_shared allocates a single chunk of
    // memory to hold both the Widget object and the control block
    // make_shared i unique są po prostu szybsze

    //ale do funkcji make nie można zapodać własnych deleterów
    //z tych funkcji nie można wywołać ctorów z std::initializer_list 

    //ale można tak

    auto initList = { 10, 20 };
    auto spv = std::make_shared<std::vector<int>>(initList);

}

void pimpl_idiom()
{
    //pimpl = pointer implementation

    //czyli w headerach trzymaj tylko nagłówki bibliotek niemoich 
    //tzn. tych , których zawartość się nie zmienia
    //a wszystkie "swoje" nagłówki z projektu includuj w plikach .cpp
    
    // w deklaracjach klas w .hpp trzymaj wskaźniki do moich innych klas



    //widget.hpp

    //takie klasy wymagają zdefiniowania destruktora(może być defaultowy)
    //a jeśli chcę używać z nimi operacji przenoszenia to muszę samemu zdefiniować move operators
    class Widget {
    public:
        Widget();
        ~Widget();

        Widget(Widget&& rhs);               // declarations
        Widget& operator=(Widget&& rhs);       // only
        
    private:
        struct Impl;    //Forward declaration
        std::unique_ptr<Impl> pImpl;// przy copy ctor pamiętaj o deep copy!!!
    };

    //widget.cpp

    //includy
    struct Widget::Impl {  }; // as before
    
    Widget::Widget()
    : pImpl(std::make_unique<Impl>())
    {} // as before
    Widget::~Widget() = default; // as before
    Widget::Widget(Widget&& rhs) = default;
    Widget& Widget::operator=(Widget&& rhs) = default;


    //ale jeśli zmienię na std::shared_ptr ,to nie muszę defniować move ctorów oraz dtora
    // wynika to z tego ,że deleter jest częścią typu std::unique_ptr i przez to trzeba 
    //definiować dtor , co ciągnie za sobą kolejne konsekwencje

    //.hpp

    class Widget {
    public:
        Widget();
    private:
        struct Impl;
        std::shared_ptr<Impl> pImpl;
    };

    //.cpp

    // For std::unique_ptr pImpl pointers, declare special member functions in
    // the class header, but implement them in the implementation file. Do this even
    // if the default function implementations are acceptable.


}

int main()
{
    //157
}