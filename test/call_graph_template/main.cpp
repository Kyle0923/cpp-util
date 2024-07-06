template <typename T>
void func_impl() {}
template <typename T>
void func(int a) {func_impl<T>()}

int main() {
    func<int>(1);
    func<float>(1);
    return 0;
}

/*

for `S: func, K: CursorKind.DECL_REF_EXPR`
child.referenced.get_template_argument_type(0).get_canonical().spelling

==========================
S: func_impl, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:2, T: void (), TK: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func_impl,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:1, T: T, TK: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func_impl,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:2, T: , TK: Invalid, #T: -1
==========================
S: func, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: void (int), TK: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:3, T: T, TK: Unexposed, #T: -1
    S: a, K: CursorKind.PARM_DECL, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: int, TK: Int, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: <dependent type>, TK: Dependent, #T: -1
            S: , K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: <dependent type>, TK: Dependent, #T: -1
                S: func_impl, K: CursorKind.OVERLOADED_DECL_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: , TK: Invalid, #T: -1
                S: T, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:4, T: T, TK: Unexposed, #T: -1
==========================
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_template/main.cpp,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:6, T: int (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:6, T: , TK: Invalid, #T: -1
        S: func, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:7, T: void, TK: Void, #T: -1
            S: func, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:7, T: void (*)(int), TK: Pointer, #T: -1
                S: func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:7, T: void (int), TK: FunctionProto, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:7, T: int, TK: Int, #T: -1
        S: func, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:8, T: void, TK: Void, #T: -1
            S: func, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:8, T: void (*)(int), TK: Pointer, #T: -1
                S: func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:8, T: void (int), TK: FunctionProto, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:8, T: int, TK: Int, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_template/main.cpp:9, T: int, TK: Int, #T: -1


*/
