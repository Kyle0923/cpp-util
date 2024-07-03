#include <iostream>
#include "header.hpp"

extern std::string my_str;

std::string get_str();
std::string& get_str_ref();
std::string* get_str_ptr();

std::string get_str() {
    return get_str_ref();
}
std::string& get_str_ref() {
    return my_str;
}

class MyOperatorClass {
private:
    int value;

public:
    // Constructor
    MyOperatorClass(int v) : value(v) {}

    // Prefix increment operator
    MyOperatorClass& operator++() {
        ++value;
        return *this;
    }

    // Postfix increment operator
    MyOperatorClass operator++(int) {
        MyOperatorClass temp = *this;
        ++(*this);
        return temp;
    }

    // Addition assignment operator
    MyOperatorClass& operator+=(int v) {
        value += v;
        return *this;
    }

    // Conversion operator to float
    explicit operator int() const {
        return value;
    }

    // Conversion operator to float
    explicit operator float() const {
        return static_cast<float>(value);
    }

    // Less-than operator
    bool operator<(const MyOperatorClass& other) const {
        return value < other.value;
    }

    // Equality operator
    bool operator==(const MyOperatorClass& other) const {
        return value == other.value;
    }

    // Subscript operator
    int operator[](int) const {
        return value;
    }

    // Binary operator for addition
    MyOperatorClass operator+(const MyOperatorClass& other) const {
        return MyOperatorClass(value + other.value);
    }

    // Friend function for output stream operator
    friend std::ostream& operator<<(std::ostream& os, const MyOperatorClass& obj) {
        os << obj.value;
        return os;
    }
};

int main() {
    int a = 0;
    Foo foo;
    Bar<int> barT;
    Bar<float> barF;
    MyOperatorClass myObj(42);
    a = (int)myObj;
    myObj++;
    myObj += -1;
    myObj = myObj + 0;
    if (myObj < 1 || myObj == 0) {
        std::cout << myObj << std::endl;
        return myObj[100];
    }
    a = foo.func1();
    a = foo.func2(a);
    a = foo.func2<int>(2);
    a = foo.func4(a, 2);
    a = (int)barF.func2(1.);
    a = barT.func2(1);
    return Foo::func5();
}

