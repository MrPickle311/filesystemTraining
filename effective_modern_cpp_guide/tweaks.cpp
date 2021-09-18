#include <string>
#include <vector>
#include <memory>
#include <list>
#include <regex>

void consider_pass_by_value__copyable_parameters_that_are_cheap_to_move_and_always_copied()
{
    class Widget 
    {
    public:
        void addName(std::string newName)//tutaj przenoszenie i kopiowanie będzie działać 
        { names.push_back(std::move(newName)); }
        // take lvalue or
        // rvalue; move it
    private:
        std::vector<std::string> names;
    };

    // In C++11, however, add
    // Name will be copy constructed only for lvalues. For rvalues, it will be move constructed.

    Widget w;

    std::string name("Bart");
    w.addName(name);//copy
    w.addName(name + "Jenne");//move

    //jeśli mamy taką klasę 

    //i jeśli mamy setter dla typu , który jest tylko przenoszalny(nie mozna go skopiować)
    class Widget1 
    {
    public:
        void setPtr(std::unique_ptr<std::string>&& ptr) // to ten setter będzie wyglądać tak
        { p = std::move(ptr); }
    private:
        std::unique_ptr<std::string> p;
    };

    //ale jeśli będzie on taki

    class Widget2 
    {
    public:
        void setPtr(std::unique_ptr<std::string> ptr) // to operacja przenoszenia będzie wykonywana oodwójnie
        { p = std::move(ptr); } // raz tu
    private:
        std::unique_ptr<std::string> p;
    };

    Widget2 we;

    we.setPtr(std::make_unique<std::string>("Modern C++")); // raz tu


    //Pass by value is worth considering only for parameters that are cheap to move.
    //You should consider pass by value only for parameters that are always copied.

    // For copyable, cheap-to-move parameters that are always copied, pass by value
    // may be nearly as efficient as pass by reference, it’s easier to implement, and it
    // can generate less object code.

    // Copying parameters via construction may be significantly more expensive
    // than copying them via assignment.

    class Password {
    public:
        explicit Password(std::string pwd) // pass by value
        : text(std::move(pwd)) {} // construct text

        void changeTo(std::string newPwd)  // pass by value
        { text = std::move(newPwd); } // assign text
    private:
        std::string text;
    };

    // Pass by value is subject to the slicing problem, so it’s typically inappropriate
    // for base class parameter types.
}

void consider_emplacement_instead_of_insertion()
{
    class Widget{};
    void killWidget(Widget* pWidget);

    // emplace backi nie tworzą temporary obiektów
    // a gdy mamy temp objects to również konstruktor oraz destruktor są dla nich wywoływane
    // push backi tworzą temp objects
    // więc emplace backi są wydajniejsze

    //kiedy jeszcze użyć emplace zamiast push
    //The argument type(s) being passed differ from the type held by the container.
    //The container is unlikely to reject the new value as a duplicate.
    //The container is unlikely to reject the new value as a duplicate.

    std::list<std::shared_ptr<Widget>> ptrs;

    //kiedy jednak używać insertów zamiast emplace'ów ?

    //tutaj jest szansa na przechwycenie wyjątku i poprawne zwolnienie zasobów przez shared_ptr
    //gdy nastąpi wyjątek dotyczący tego ,że nie można już rozszerzyć wektora 
    ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));

    //tutaj nic nie zostanie zwolnione
    ptrs.emplace_back(new Widget, killWidget);

    //ale powyższy kod i tak nie jest idealny

    //lepiej i bezpieczniej zrobić to tak

     std::shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.push_back(std::move(spw));


    std::shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.emplace_back(std::move(spw));

    // In principle, emplacement functions should sometimes be more efficient than
    // their insertion counterparts, and they should never be less efficient.

    // In practice, they’re most likely to be faster when (1) the value being added is
    // constructed into the container, not assigned; (2) the argument type(s) passed
    // differ from the type held by the container; and (3) the container won’t reject
    // the value being added due to it being a duplicate.

    // Emplacement functions may perform type conversions that would be rejected
    // by insertion functions.

    std::vector<std::regex> regexes;

    regexes.emplace_back(nullptr);//dlaczego kompilator nie krzyczy ? 
    //bo jest tutaj niejawne castowanie na const char* o zerowym rozmiarze

    regexes.push_back(nullptr); //tutaj kompilator zaprotestuje!
}

int main()
{

}