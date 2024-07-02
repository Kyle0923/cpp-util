#ifndef HEADER_HPP_
#define HEADER_HPP_

class Foo {
public:
    int func1();
    int func2(int a);
    template<class U>
    int func2(U b);
    static int func4(int a, int b);
    static int func5();
}; // class Foo;

template<class U>
int Foo::func2(U b) {return b;}


template<class T>
class Bar {
public:
    T func1(T a) {return a;}
    T func2(T a) {return func1(a);}
}; // class Bar;

#endif // HEADER_HPP_
