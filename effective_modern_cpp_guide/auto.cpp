#include <functional>

void prefer_use_auto()
{
    // An auto-declared variable holding a closure has the same type as the closure, and as such
    // it uses only as much memory as the closure requires. The type of a std::function-
    // declared variable holding a closure is an instantiation of the std::function tem‐
    // plate, and that has a fixed size for any given signature. This size may not be adequate
    // // for the closure it’s asked to store, and when that’s the case, the std::function con‐
    // structor will allocate heap memory to store the closure.
    // The result is that the std::function object typically uses more memory than the auto-declared object.

    // td::function object is almost certain to be
    // slower than calling it via an auto-declared object.

    std::vector<int> v;

    unsigned sz = v.size();// brak przenośności miedzy platformami

    auto s1 = v.size(); // jest przenośność

    // auto variables must be initialized, are generally immune to type mismatches
    // that can lead to portability or efficiency problems, can ease the process of
    // refactoring, and typically require less typing than variables with explicitly
    // specified types.
}

class Widget{};

std::vector<bool> features(const Widget& w);

void processWidget(Widget& w, bool prior);

void use_the_explicit_typed_initializer_idiom_when_dedcues_undesired_types()
{
    Widget w;

    bool highPriority = features(w)[5]; // is w high priority?

    processWidget(w, highPriority);

    auto highPriority1 = features(w)[5];//is it priority?

    // highPriority1 doesn’t have the value of bit 5 of the std::vector<bool>
    // returned by features at all !.
    // its implementation depend

    processWidget(w, highPriority1); // undefined behavior!

    //Reason , dla ciekwaskich
    // The call to features returns a temporary std::vector<bool> object. This object
    // has no name, but for purposes of this discussion, I’ll call it temp. operator[] is
    // invoked on temp, and the std::vector<bool>::reference it returns contains a
    // pointer to a word in the data structure holding the bits that are managed by temp,
    // plus the offset into that word corresponding to bit 5. highPriority is a copy of this
    // std::vector<bool>::reference object, so highPriority, too, contains a pointer
    // to a word in temp, plus the offset corresponding to bit 5. At the end of the statement,
    // temp is destroyed, because it’s a temporary object. Therefore, highPriority contains
    // a dangling pointer, and that’s the cause of the undefined behavior
    // operator[] returns something kind of Proxy

    // In the code using auto, the type of
    // highPriority is no longer bool. Though std::vector<bool> conceptually holds
    // bools, operator[] for std::vector<bool> doesn’t return a reference to an element
    // of the container

    //operator[] for std::vector<bool> returns an object that acts like a bool&.

    //jeśli mam zwracane proxy , to warto rozważyć użycia tego idiomu

    auto highPriority2 = static_cast<bool>(features(w)[5]);

    //czyli do niejawnych konwersji preferuj static_cast

    double d = 0.5;

    auto index = static_cast<int>(d * features(w).size());

    //sprawdzaj czy klasa nie zwraca proxy
}

int main()
{
    prefer_use_auto();
    use_the_explicit_typed_initializer_idiom_when_dedcues_undesired_types();
}