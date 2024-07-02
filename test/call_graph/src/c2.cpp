#include <string>
#include "header.hpp"

static std::string my_str = "TEST";

int Foo::func1(){return my_str.length();}
int Foo::func2(int a) {return a;}
int Foo::func4(int a, int b) {return a + b;}
int Foo::func5() {return func4(1,func4(1,2));}
