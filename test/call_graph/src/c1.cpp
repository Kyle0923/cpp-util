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

/*

==========================
S: Foo, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:6, T: Foo, TK: Record, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:7, T: , TK: Invalid, #T: -1
    S: func1, K: CursorKind.CXX_METHOD, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:8, T: int (), TK: FunctionProto, #T: -1
    S: func2, K: CursorKind.CXX_METHOD, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:9, T: int (int), TK: FunctionProto, #T: -1
        S: , K: CursorKind.PARM_DECL, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:9, T: int, TK: Int, #T: -1
    S: func2, K: CursorKind.FUNCTION_TEMPLATE, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:11, T: U (U), TK: FunctionProto, #T: -1
        S: U, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:10, T: U, TK: Unexposed, #T: -1
        S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:11, T: U, TK: Unexposed, #T: -1
        S: b, K: CursorKind.PARM_DECL, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:11, T: U, TK: Unexposed, #T: -1
            S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:11, T: U, TK: Unexposed, #T: -1
    S: func4, K: CursorKind.CXX_METHOD, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:12, T: int (int, int), TK: FunctionProto, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: func4,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:12, T: int, TK: Int, #T: -1
        S: b, K: CursorKind.PARM_DECL, PS: func4,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:12, T: int, TK: Int, #T: -1
    S: func5, K: CursorKind.CXX_METHOD, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:13, T: int (), TK: FunctionProto, #T: -1
==========================
S: func2_impl, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: U (U), TK: FunctionProto, #T: -1
    S: U, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func2_impl,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:16, T: U, TK: Unexposed, #T: -1
    S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: U, TK: Unexposed, #T: -1
    S: b, K: CursorKind.PARM_DECL, PS: func2_impl,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: U, TK: Unexposed, #T: -1
        S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: U, TK: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func2_impl,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: , TK: Invalid, #T: -1
            S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:17, T: U, TK: Unexposed, #T: -1
==========================
S: func2, K: CursorKind.FUNCTION_TEMPLATE, PS: Foo,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: U (U), TK: FunctionProto, #T: -1
    S: U, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:19, T: U, TK: Unexposed, #T: -1
    S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: U, TK: Unexposed, #T: -1
    S: class Foo, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: Foo, TK: Record, #T: -1
    S: b, K: CursorKind.PARM_DECL, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: U, TK: Unexposed, #T: -1
        S: U, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: U, TK: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: <dependent type>, TK: Dependent, #T: -1
                S: , K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: <overloaded function type>, TK: Overload, #T: -1
                    S: func2_impl, K: CursorKind.OVERLOADED_DECL_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: , TK: Invalid, #T: -1
                S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:20, T: U, TK: Unexposed, #T: -1
==========================
S: Bar, K: CursorKind.CLASS_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:24, T: , TK: Invalid, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: Bar,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:23, T: T, TK: Unexposed, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: Bar,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:25, T: , TK: Invalid, #T: -1
    S: func1, K: CursorKind.CXX_METHOD, PS: Bar,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: T (T), TK: FunctionProto, #T: -1
        S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: T, TK: Unexposed, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: func1,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: T, TK: Unexposed, #T: -1
            S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: T, TK: Unexposed, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func1,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: , TK: Invalid, #T: -1
                S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:26, T: T, TK: Unexposed, #T: -1
    S: func2, K: CursorKind.CXX_METHOD, PS: Bar,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: T (T), TK: FunctionProto, #T: -1
        S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: T, TK: Unexposed, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: T, TK: Unexposed, #T: -1
            S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: T, TK: Unexposed, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: , TK: Invalid, #T: -1
                S: func1, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: <dependent type>, TK: Dependent, #T: -1
                    S: func1, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: <bound member function type>, TK: Unexposed, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/header.hpp:27, T: T, TK: Unexposed, #T: -1
==========================
S: my_str, K: CursorKind.VAR_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:4, T: std::string, TK: Elaborated, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:4, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:4, T: std::string, TK: Typedef, #T: -1
==========================
S: get_str, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:6, T: std::string (), TK: FunctionProto, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:6, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:6, T: std::string, TK: Typedef, #T: -1
==========================
S: get_str_ref, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: std::string &(), TK: FunctionProto, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: std::string, TK: Typedef, #T: -1
==========================
S: get_str_ptr, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: std::string *(), TK: FunctionProto, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: std::string, TK: Typedef, #T: -1
==========================
S: get_str, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: std::string (), TK: FunctionProto, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: std::string, TK: Typedef, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: get_str,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: , TK: Invalid, #T: -1
            S: basic_string, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: std::string, TK: Elaborated, #T: -1
                S: get_str_ref, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: const basic_string<char>, TK: Elaborated, #T: -1
                    S: get_str_ref, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: std::string, TK: Elaborated, #T: -1
                        S: get_str_ref, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: std::string &(*)(), TK: Pointer, #T: -1
                            S: get_str_ref, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: std::string &(), TK: FunctionProto, #T: -1
==========================
S: get_str_ref, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:13, T: std::string &(), TK: FunctionProto, #T: -1
    S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:13, T: , TK: Invalid, #T: -1
    S: std::string, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:13, T: std::string, TK: Typedef, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: get_str_ref,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:13, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:14, T: , TK: Invalid, #T: -1
            S: my_str, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:14, T: std::string, TK: Elaborated, #T: -1
==========================
S: MyOperatorClass, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:17, T: MyOperatorClass, TK: Record, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:18, T: , TK: Invalid, #T: -1
    S: value, K: CursorKind.FIELD_DECL, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:19, T: int, TK: Int, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: , TK: Invalid, #T: -1
    S: MyOperatorClass, K: CursorKind.CONSTRUCTOR, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: void (int), TK: FunctionProto, #T: -1
        S: v, K: CursorKind.PARM_DECL, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: int, TK: Int, #T: -1
        S: value, K: CursorKind.MEMBER_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: int, TK: Int, #T: -1
        S: v, K: CursorKind.UNEXPOSED_EXPR, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: int, TK: Int, #T: -1
            S: v, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: int, TK: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: , TK: Invalid, #T: -1
    S: operator++, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:26, T: MyOperatorClass &(), TK: FunctionProto, #T: -1
        S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:26, T: MyOperatorClass, TK: Record, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator++,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:26, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: int, TK: Int, #T: -1
                S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: int, TK: Int, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:28, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:28, T: MyOperatorClass, TK: Record, #T: -1
                    S: , K: CursorKind.CXX_THIS_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:28, T: MyOperatorClass *, TK: Pointer, #T: -1
    S: operator++, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: MyOperatorClass (int), TK: FunctionProto, #T: -1
        S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: MyOperatorClass, TK: Record, #T: -1
        S: , K: CursorKind.PARM_DECL, PS: operator++,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: int, TK: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator++,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: , TK: Invalid, #T: -1
                S: temp, K: CursorKind.VAR_DECL, PS: operator++,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: MyOperatorClass, TK: Record, #T: -1
                    S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: temp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: const MyOperatorClass, TK: Elaborated, #T: -1
                            S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: MyOperatorClass, TK: Record, #T: -1
                                S: , K: CursorKind.CXX_THIS_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: MyOperatorClass *, TK: Pointer, #T: -1
            S: operator++, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass, TK: Elaborated, #T: -1
                S: , K: CursorKind.PAREN_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass, TK: Record, #T: -1
                    S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass, TK: Record, #T: -1
                        S: , K: CursorKind.CXX_THIS_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass *, TK: Pointer, #T: -1
                S: operator++, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass &(*)(), TK: Pointer, #T: -1
                    S: operator++, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: MyOperatorClass &(), TK: FunctionProto, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:35, T: , TK: Invalid, #T: -1
                S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:35, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: temp, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:35, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: temp, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:35, T: MyOperatorClass, TK: Elaborated, #T: -1
    S: operator+=, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: MyOperatorClass &(int), TK: FunctionProto, #T: -1
        S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: MyOperatorClass, TK: Record, #T: -1
        S: v, K: CursorKind.PARM_DECL, PS: operator+=,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: int, TK: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator+=,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.COMPOUND_ASSIGNMENT_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: int, TK: Int, #T: -1
                S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: int, TK: Int, #T: -1
                S: v, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: int, TK: Int, #T: -1
                    S: v, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: int, TK: Int, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:41, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:41, T: MyOperatorClass, TK: Record, #T: -1
                    S: , K: CursorKind.CXX_THIS_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:41, T: MyOperatorClass *, TK: Pointer, #T: -1
    S: operator int, K: CursorKind.CONVERSION_FUNCTION, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:45, T: int () const, TK: FunctionProto, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator int,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:45, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:46, T: , TK: Invalid, #T: -1
                S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:46, T: int, TK: Int, #T: -1
                    S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:46, T: const int, TK: Int, #T: -1
    S: operator float, K: CursorKind.CONVERSION_FUNCTION, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:50, T: float () const, TK: FunctionProto, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator float,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:50, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:51, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.CXX_STATIC_CAST_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:51, T: float, TK: Float, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:51, T: float, TK: Float, #T: -1
                        S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:51, T: int, TK: Int, #T: -1
                            S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:51, T: const int, TK: Int, #T: -1
    S: operator<, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:55, T: bool (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
        S: other, K: CursorKind.PARM_DECL, PS: operator<,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:55, T: const MyOperatorClass &, TK: LValueReference, #T: -1
            S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:55, T: MyOperatorClass, TK: Record, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator<,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:55, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: bool, TK: Bool, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: int, TK: Int, #T: -1
                        S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: const int, TK: Int, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: int, TK: Int, #T: -1
                        S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: const int, TK: Int, #T: -1
                            S: other, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:56, T: const MyOperatorClass, TK: Elaborated, #T: -1
    S: operator==, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:60, T: bool (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
        S: other, K: CursorKind.PARM_DECL, PS: operator==,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:60, T: const MyOperatorClass &, TK: LValueReference, #T: -1
            S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:60, T: MyOperatorClass, TK: Record, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator==,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:60, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: bool, TK: Bool, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: int, TK: Int, #T: -1
                        S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: const int, TK: Int, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: int, TK: Int, #T: -1
                        S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: const int, TK: Int, #T: -1
                            S: other, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:61, T: const MyOperatorClass, TK: Elaborated, #T: -1
    S: operator[], K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:65, T: int (int) const, TK: FunctionProto, #T: -1
        S: , K: CursorKind.PARM_DECL, PS: operator[],L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:65, T: int, TK: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator[],L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:65, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:66, T: , TK: Invalid, #T: -1
                S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:66, T: int, TK: Int, #T: -1
                    S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:66, T: const int, TK: Int, #T: -1
    S: operator+, K: CursorKind.CXX_METHOD, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:70, T: MyOperatorClass (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
        S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:70, T: MyOperatorClass, TK: Record, #T: -1
        S: other, K: CursorKind.PARM_DECL, PS: operator+,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:70, T: const MyOperatorClass &, TK: LValueReference, #T: -1
            S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:70, T: MyOperatorClass, TK: Record, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: operator+,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:70, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.CXX_FUNCTIONAL_CAST_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: MyOperatorClass, TK: Record, #T: -1
                    S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: int, TK: Int, #T: -1
                            S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: int, TK: Int, #T: -1
                                S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: const int, TK: Int, #T: -1
                            S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: int, TK: Int, #T: -1
                                S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: const int, TK: Int, #T: -1
                                    S: other, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:71, T: const MyOperatorClass, TK: Elaborated, #T: -1
    S: , K: CursorKind.FRIEND_DECL, PS: MyOperatorClass,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: , TK: Invalid, #T: -1
        S: operator<<, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: std::ostream &(std::ostream &, const MyOperatorClass &), TK: FunctionProto, #T: -1
            S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: , TK: Invalid, #T: -1
            S: std::ostream, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: std::ostream, TK: Typedef, #T: -1
            S: os, K: CursorKind.PARM_DECL, PS: operator<<,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: std::ostream &, TK: LValueReference, #T: -1
                S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: , TK: Invalid, #T: -1
                S: std::ostream, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: std::ostream, TK: Typedef, #T: -1
            S: obj, K: CursorKind.PARM_DECL, PS: operator<<,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: const MyOperatorClass &, TK: LValueReference, #T: -1
                S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: MyOperatorClass, TK: Record, #T: -1
            S: , K: CursorKind.COMPOUND_STMT, PS: operator<<,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:75, T: , TK: Invalid, #T: -1
                S: operator<<, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: __ostream_type, TK: Elaborated, #T: -1
                    S: os, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: std::ostream, TK: Elaborated, #T: -1
                    S: operator<<, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: __ostream_type &(*)(int), TK: Pointer, #T: -1
                        S: operator<<, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: __ostream_type &(int), TK: FunctionProto, #T: -1
                    S: value, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: int, TK: Int, #T: -1
                        S: value, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: const int, TK: Int, #T: -1
                            S: obj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:76, T: const MyOperatorClass, TK: Elaborated, #T: -1
                S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:77, T: , TK: Invalid, #T: -1
                    S: os, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:77, T: std::ostream, TK: Elaborated, #T: -1
==========================
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:81, T: int (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:81, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:82, T: , TK: Invalid, #T: -1
            S: a, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:82, T: int, TK: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: a,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:82, T: int, TK: Int, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:83, T: , TK: Invalid, #T: -1
            S: foo, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:83, T: Foo, TK: Elaborated, #T: -1
                S: class Foo, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:83, T: Foo, TK: Record, #T: -1
                S: Foo, K: CursorKind.CALL_EXPR, PS: foo,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:83, T: Foo, TK: Elaborated, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:84, T: , TK: Invalid, #T: -1
            S: barT, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:84, T: Bar<int>, TK: Elaborated, #T: -1
                S: Bar, K: CursorKind.TEMPLATE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:84, T: , TK: Invalid, #T: -1
                S: Bar, K: CursorKind.CALL_EXPR, PS: barT,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:84, T: Bar<int>, TK: Elaborated, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:85, T: , TK: Invalid, #T: -1
            S: barF, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:85, T: Bar<float>, TK: Elaborated, #T: -1
                S: Bar, K: CursorKind.TEMPLATE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:85, T: , TK: Invalid, #T: -1
                S: Bar, K: CursorKind.CALL_EXPR, PS: barF,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:85, T: Bar<float>, TK: Elaborated, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:86, T: , TK: Invalid, #T: -1
            S: myObj, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:86, T: MyOperatorClass, TK: Elaborated, #T: -1
                S: class MyOperatorClass, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:86, T: MyOperatorClass, TK: Record, #T: -1
                S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: myObj,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:86, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:86, T: int, TK: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: int, TK: Int, #T: -1
            S: , K: CursorKind.CSTYLE_CAST_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: int, TK: Int, #T: -1
                S: operator int, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: int, TK: Int, #T: -1
                    S: operator int, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: int, TK: Int, #T: -1
                        S: operator int, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: , T: <bound member function type>, TK: Unexposed, #T: -1
                            S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: const MyOperatorClass, TK: Record, #T: -1
                                S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:87, T: MyOperatorClass, TK: Elaborated, #T: -1
        S: operator++, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:88, T: MyOperatorClass, TK: Elaborated, #T: -1
            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:88, T: MyOperatorClass, TK: Elaborated, #T: -1
            S: operator++, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:88, T: MyOperatorClass (*)(int), TK: Pointer, #T: -1
                S: operator++, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:88, T: MyOperatorClass (int), TK: FunctionProto, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: , T: int, TK: Int, #T: -1
        S: operator+=, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: MyOperatorClass, TK: Elaborated, #T: -1
            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: MyOperatorClass, TK: Elaborated, #T: -1
            S: operator+=, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: MyOperatorClass &(*)(int), TK: Pointer, #T: -1
                S: operator+=, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: MyOperatorClass &(int), TK: FunctionProto, #T: -1
            S: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: int, TK: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:89, T: int, TK: Int, #T: -1
        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
            S: operator=, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
                S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
                S: operator=, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass &(*)(MyOperatorClass &&) noexcept, TK: Pointer, #T: -1
                    S: operator=, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass &(MyOperatorClass &&) noexcept, TK: FunctionProto, #T: -1
                S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: operator+, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: const MyOperatorClass, TK: Record, #T: -1
                            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: operator+, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass (*)(const MyOperatorClass &) const, TK: Pointer, #T: -1
                            S: operator+, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: MyOperatorClass (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: const MyOperatorClass, TK: Elaborated, #T: -1
                            S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: const MyOperatorClass, TK: Elaborated, #T: -1
                                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:90, T: int, TK: Int, #T: -1
        S: , K: CursorKind.IF_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool, TK: Bool, #T: -1
                S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool, TK: Bool, #T: -1
                    S: operator<, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool, TK: Bool, #T: -1
                        S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Record, #T: -1
                            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: operator<, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool (*)(const MyOperatorClass &) const, TK: Pointer, #T: -1
                            S: operator<, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Elaborated, #T: -1
                            S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Elaborated, #T: -1
                                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: int, TK: Int, #T: -1
                    S: operator==, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool, TK: Bool, #T: -1
                        S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Record, #T: -1
                            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: operator==, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool (*)(const MyOperatorClass &) const, TK: Pointer, #T: -1
                            S: operator==, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: bool (const MyOperatorClass &) const, TK: FunctionProto, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Elaborated, #T: -1
                            S: MyOperatorClass, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: const MyOperatorClass, TK: Elaborated, #T: -1
                                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: int, TK: Int, #T: -1
            S: , K: CursorKind.COMPOUND_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:91, T: , TK: Invalid, #T: -1
                S: operator<<, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: __ostream_type, TK: Elaborated, #T: -1
                    S: operator<<, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: std::ostream, TK: Elaborated, #T: -1
                        S: cout, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: ostream, TK: Elaborated, #T: -1
                            S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: , TK: Invalid, #T: -1
                        S: operator<<, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: std::ostream &(*)(std::ostream &, const MyOperatorClass &), TK: Pointer, #T: -1
                            S: operator<<, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: std::ostream &(std::ostream &, const MyOperatorClass &), TK: FunctionProto, #T: -1
                        S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: const MyOperatorClass, TK: Elaborated, #T: -1
                            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: MyOperatorClass, TK: Elaborated, #T: -1
                    S: operator<<, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: __ostream_type &(*)(__ostream_type &(*)(__ostream_type &)), TK: Pointer, #T: -1
                        S: operator<<, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: __ostream_type &(__ostream_type &(*)(__ostream_type &)), TK: FunctionProto, #T: -1
                    S: endl, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: basic_ostream<char, char_traits<char>> &(*)(basic_ostream<char, char_traits<char>> &), TK: Pointer, #T: -1
                        S: endl, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: basic_ostream<char, char_traits<char>> &(basic_ostream<char, char_traits<char>> &), TK: FunctionProto, #T: -1
                            S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:92, T: , TK: Invalid, #T: -1
                S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: , TK: Invalid, #T: -1
                    S: operator[], K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: int, TK: Int, #T: -1
                        S: myObj, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: const MyOperatorClass, TK: Record, #T: -1
                            S: myObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: MyOperatorClass, TK: Elaborated, #T: -1
                        S: operator[], K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: int (*)(int) const, TK: Pointer, #T: -1
                            S: operator[], K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: int (int) const, TK: FunctionProto, #T: -1
                        S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:93, T: int, TK: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:95, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:95, T: int, TK: Int, #T: -1
            S: func1, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:95, T: int, TK: Int, #T: -1
                S: func1, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:95, T: <bound member function type>, TK: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:95, T: Foo, TK: Elaborated, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: int, TK: Int, #T: -1
            S: func2, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: int, TK: Int, #T: -1
                S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: <bound member function type>, TK: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: Foo, TK: Elaborated, #T: -1
                S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: int, TK: Int, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:96, T: int, TK: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: int, TK: Int, #T: -1
            S: func2, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: int, TK: Unexposed, #T: -1
                S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: <bound member function type>, TK: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: Foo, TK: Elaborated, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:97, T: int, TK: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
            S: func4, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
                S: func4, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int (*)(int, int), TK: Pointer, #T: -1
                    S: func4, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int (int, int), TK: FunctionProto, #T: -1
                        S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: Foo, TK: Elaborated, #T: -1
                S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:98, T: int, TK: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: int, TK: Int, #T: -1
            S: , K: CursorKind.CSTYLE_CAST_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: int, TK: Int, #T: -1
                S: func2, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: int, TK: Int, #T: -1
                    S: func2, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: float, TK: Unexposed, #T: -1
                        S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: <bound member function type>, TK: Unexposed, #T: -1
                            S: barF, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: Bar<float>, TK: Elaborated, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: float, TK: Unexposed, #T: -1
                            S: , K: CursorKind.FLOATING_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:99, T: double, TK: Double, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: int, TK: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: int, TK: Int, #T: -1
            S: func2, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: int, TK: Unexposed, #T: -1
                S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: <bound member function type>, TK: Unexposed, #T: -1
                    S: barT, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: Bar<int>, TK: Elaborated, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:100, T: int, TK: Int, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:101, T: , TK: Invalid, #T: -1
            S: func5, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:101, T: int, TK: Int, #T: -1
                S: func5, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:101, T: int (*)(), TK: Pointer, #T: -1
                    S: func5, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:101, T: int (), TK: FunctionProto, #T: -1
                        S: class Foo, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:101, T: Foo, TK: Record, #T: -1

 */
