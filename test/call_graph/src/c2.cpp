#include "header.hpp"

std::string my_str = "TEST";

int Foo::func1(){
    if (true)
        return func2(1);
    else
        return my_str.length();
}
int Foo::func2(int a) {
    if (a == 0)
        return func1();
    else if (a == 1)
        return func2(a-1);
    else if (a == 2)
        return func2(a-1);
    else
        return 1;
}
int Foo::func4(int a, int b) {return a + b;}
int Foo::func5() {return func4(1,func4(1,2));}
