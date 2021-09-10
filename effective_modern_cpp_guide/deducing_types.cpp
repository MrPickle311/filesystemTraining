#include <iostream>

template<typename T>
void f(const T& param) {}

template<typename T>
void h(const T* param) {}

//universal reference
template<typename T>
void abc(T&& param) {}

template<typename T>
void kkk(T param) {}//ignore any decorators , only passing by copy 

//how does it work ? 
// let see...
// 
// template<typename T>
// void abc(ParamtType param) {}
// 
//  abc(expr);
//  If expr is an lvalue, both T and ParamType are deduced to be lvalue references.
//  If expr is an rvalue, the “normal” rules apply.

//the following template returns size of an array

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept // below on
{
    return N;
}

void someFunc(int, double) {}

void template_type_deduction()
{
    int x = 45;
    const int cx = x; 
    const int& rx = x;  
    volatile const int v = 456;

    f(x);//function arg type will be decorated by const & 
    f(cx);
    f(rx);
    f(v);
    h(&x);//it works also with pointers

    //universal ref

    abc(x); // lval ref
    abc(cx); // const lval ref
    abc(rx); //as above
    abc(45); //rval

    //each is passed by copy
    kkk(x);
    kkk(cx);
    kkk(rx);
    kkk(45);

    const char* const ptr = "Fun with pointers";
    kkk(ptr);// const char * , only pointer will be copied ,  not string 

    // but if we have the following variables
    const char name[] = "J. P. Briggs";
    const char* ptrToName = name;

    //their template params will be const char*
    kkk(name);
    kkk(ptrToName);
    // functions can’t declare parameters that are
    // truly arrays, they can declare parameters that are references to arrays!

    f(name);//so here , deduced type is an array

    int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
    std::cout << arraySize(keyVals) << std::endl;

    //function type deduction
    //Function types can decay into function pointers
    kkk(someFunc); // ptr to a function
    f(someFunc); // here is referene to a function
}

void auto_type_deduction()
{
    //auto type deduction is template type deduction
    // When a variable is declared using auto, auto plays the role of T in the template,

    //normal copy-paste , auto == T
    auto x = 345;
    const auto cx = x;
    const auto& rx = x;

    //universal ref

    auto&& uref1 = x;//lval ref
    auto&& uref2 = cx;// const lval ref
    auto&& uref3 = rx;// const lval ref

    const char name[] = "J. P. Briggs";

    auto arr1 = name;
    auto& arr2 = name; // arr2's type is const char (&)[13]

    auto func1 = someFunc; // func1's type is void (*)(int, double)

    auto x1{27}; //int 
    auto x2 = {27};/// std::initializer_list<int>

    // The treatment of braced initializers is the only way in which auto type deduction and
    // template type deduction differ. When an auto–declared variable is initialized with a
    // braced initializer, the deduced type is an instantiation of std::initializer_list.
    // But if the corresponding template is passed the same initializer, type deduction fails,

}

class Widget{};

// In this template, we don’t know what type of container we’re operating on, and that
// means we’re equally ignorant of the type of index objects it uses. Employing pass-by-
// value for objects of an unknown type generally risks the performance hit of unneces‐
// sary copying

template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i)
{
    authenticateUser();
    return std::forward<Container>(c)[i];//std::forward must to have
}

// For lvalue expressions of type T other than names, decltype always reports a
// type of T&.

decltype(auto) f1()
{
    int x = 0; // decltype(x) is int, so f1 returns int
    return x;
}

decltype(auto) f2()
{
    int x = 0; // decltype((x)) is int&, so f2 returns int&
    return (x);
}

void decltype_some_info()
{
    Widget w;
    const Widget& cw = w;

    auto w1 = cw;//copy
    decltype(auto) w2 = cw;//const ref
}

int main()
{
    template_type_deduction();
    auto_type_deduction();
    decltype_some_info();
    //37 auto
}

