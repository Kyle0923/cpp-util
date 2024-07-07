
template <typename T>
void func_impl() {}
template <typename T, int N>
void func(int a) {func_impl<T>()}

template <typename T>
class MyClass {
    T method(T a) {func_impl<T>(); return a;}
};

void myFunction() {}

// Template class with a function pointer as a non-type template parameter
template<void (*Func)()>
void callFunction() {
    Func(); // Call the function passed as a template argument
}

typedef int MY_SPECIAL;

int main() {
    func<int, 1>(1);
    func<MY_SPECIAL, 1>(1);
    callFunction<myFunction>();
    return 0;
}

/*

for `S: func, K: CursorKind.DECL_REF_EXPR`
child.referenced.get_template_argument_type(0).get_canonical().spelling

==========================
S: func_impl, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:3, T: void (), TK: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func_impl,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:2, T: T, TK: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func_impl,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:3, T: , TK: Invalid, #T: -1
==========================
S: func, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: void (int), TK: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: T, TK: Unexposed, #T: -1
    S: N, K: CursorKind.TEMPLATE_NON_TYPE_PARAMETER, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: int, TK: Int, #T: -1
    S: a, K: CursorKind.PARM_DECL, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: int, TK: Int, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: <dependent type>, TK: Dependent, #T: -1
            S: , K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: <dependent type>, TK: Dependent, #T: -1
                S: func_impl, K: CursorKind.OVERLOADED_DECL_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: , TK: Invalid, #T: -1
                S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:5, T: T, TK: Unexposed, #T: -1
==========================
S: MyClass, K: CursorKind.CLASS_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:8, T: , TK: Invalid, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: MyClass,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:7, T: T, TK: Unexposed, #T: -1
    S: method, K: CursorKind.CXX_METHOD, PS: MyClass,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T (T), TK: FunctionProto, #T: -1
        S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T, TK: Unexposed, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: method,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T, TK: Unexposed, #T: -1
            S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T, TK: Unexposed, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: method,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: <dependent type>, TK: Dependent, #T: -1
                S: , K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: <dependent type>, TK: Dependent, #T: -1
                    S: func_impl, K: CursorKind.OVERLOADED_DECL_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: , TK: Invalid, #T: -1
                    S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T, TK: Unexposed, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: , TK: Invalid, #T: -1
                S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: T, TK: Unexposed, #T: -1
==========================
S: myFunction, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:12, T: void (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: myFunction,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:12, T: , TK: Invalid, #T: -1
==========================
S: callFunction, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:16, T: void (), TK: FunctionProto, #T: -1
    S: Func, K: CursorKind.TEMPLATE_NON_TYPE_PARAMETER, PS: callFunction,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:15, T: void (*)(), TK: Pointer, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: callFunction,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:16, T: , TK: Invalid, #T: -1
        S: Func, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:17, T: void, TK: Void, #T: -1
            S: Func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:17, T: void (*)(), TK: Pointer, #T: -1
==========================
S: MY_SPECIAL, K: CursorKind.TYPEDEF_DECL, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:20, T: MY_SPECIAL, TK: Typedef, #T: -1
==========================
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:22, T: int (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:22, T: , TK: Invalid, #T: -1
        S: func, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:23, T: void, TK: Void, #T: -1
            S: func, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:23, T: void (*)(int), TK: Pointer, #T: -1
                S: func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:23, T: void (int), TK: FunctionProto, #T: -1
                    S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:23, T: int, TK: Int, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:23, T: int, TK: Int, #T: -1
        S: func, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: void, TK: Void, #T: -1
            S: func, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: void (*)(int), TK: Pointer, #T: -1
                S: func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: void (int), TK: FunctionProto, #T: -1
                    S: MY_SPECIAL, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: MY_SPECIAL, TK: Typedef, #T: -1
                    S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: int, TK: Int, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:24, T: int, TK: Int, #T: -1
        S: callFunction, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:25, T: void, TK: Void, #T: -1
            S: callFunction, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:25, T: void (*)(), TK: Pointer, #T: -1
                S: callFunction, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:25, T: void (), TK: FunctionProto, #T: -1
                    S: myFunction, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:25, T: void (), TK: FunctionProto, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:26, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:26, T: int, TK: Int, #T: -1


*/
