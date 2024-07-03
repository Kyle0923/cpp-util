#ifndef HEADER_HPP_
#define HEADER_HPP_

#include <string>

class Foo {
public:
    int func1();
    int func2(int);
    template<class U>
    U func2(U b);
    static int func4(int a, int b);
    static int func5();
}; // class Foo;

template<class U>
U func2_impl(U b) {return b;}

template<class U>
U Foo::func2(U b) {return func2_impl(b);}


template<class T>
class Bar {
public:
    T func1(T a) {return a;}
    T func2(T a) {return func1(a);}
}; // class Bar;

#endif // HEADER_HPP_
