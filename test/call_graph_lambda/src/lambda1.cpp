#include <functional>
// #include "header.hpp"

int forward_call( std::function<int(void)> func) {
    return func();
}

typedef int (*func_ptr_t)(void);

int main() {
    int (*ptr)(float) = [](float) {return 0;};
    forward_call(
        []() {return 0;}
    );
    ptr(1.);
    return 0;
}

/*

==========================
S: forward_call, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:4, T: int (std::function<int ()>), TK: FunctionProto, #T: -1
    S: func, K: CursorKind.PARM_DECL, PS: forward_call,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:4, T: std::function<int ()>, TK: Elaborated, #T: 1
        S: std, K: CursorKind.NAMESPACE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:4, T: , TK: Invalid, #T: -1
        S: function, K: CursorKind.TEMPLATE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:4, T: , TK: Invalid, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: forward_call,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:4, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: , TK: Invalid, #T: -1
            S: operator(), K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: int, TK: Unexposed, #T: -1
                S: func, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: const std::function<int ()>, TK: Record, #T: 1
                    S: func, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: std::function<int ()>, TK: Elaborated, #T: 1
                S: operator(), K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: int (*)() const, TK: Pointer, #T: -1
                    S: operator(), K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:5, T: int () const, TK: FunctionProto, #T: -1
==========================
S: func_ptr_t, K: CursorKind.TYPEDEF_DECL, PS: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:8, T: func_ptr_t, TK: Typedef, #T: -1
==========================
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: int (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: , TK: Invalid, #T: -1
            S: ptr, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: int (*)(float), TK: Pointer, #T: -1
                S: , K: CursorKind.PARM_DECL, PS: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: float, TK: Float, #T: -1
                S: , K: CursorKind.UNEXPOSED_EXPR, PS: ptr,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: int (*)(float), TK: Pointer, #T: -1
                    S: operator int (*)(float), K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: int (*)(float), TK: Pointer, #T: -1
                        S: operator int (*)(float), K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: int (*)(float), TK: Pointer, #T: -1
                            S: operator int (*)(float), K: CursorKind.MEMBER_REF_EXPR, PS: ,L: , T: <bound member function type>, TK: Unexposed, #T: -1
                                S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: const (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11:25), TK: Record, #T: -1
                                    S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11:25), TK: Record, #T: -1
                                        S: , K: CursorKind.LAMBDA_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11:25), TK: Record, #T: -1
                                            S: , K: CursorKind.PARM_DECL, PS: operator(),L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: float, TK: Float, #T: -1
                                            S: , K: CursorKind.COMPOUND_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: , TK: Invalid, #T: -1
                                                S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: , TK: Invalid, #T: -1
                                                    S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:11, T: int, TK: Int, #T: -1
        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: int, TK: Int, #T: -1
            S: forward_call, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: int, TK: Int, #T: -1
                S: forward_call, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: int (*)(std::function<int ()>), TK: Pointer, #T: -1
                    S: forward_call, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: int (std::function<int ()>), TK: FunctionProto, #T: -1
                S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                    S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                            S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                                S: function, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                                    S: function, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: std::function<int ()>, TK: Elaborated, #T: 1
                                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13:9), TK: Record, #T: -1
                                            S: , K: CursorKind.LAMBDA_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13:9), TK: Record, #T: -1
                                                S: , K: CursorKind.COMPOUND_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: , TK: Invalid, #T: -1
                                                    S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: , TK: Invalid, #T: -1
                                                        S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:13, T: int, TK: Int, #T: -1
        S: ptr, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:15, T: int, TK: Int, #T: -1
            S: ptr, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:15, T: int (*)(float), TK: Pointer, #T: -1
                S: ptr, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:15, T: int (*)(float), TK: Pointer, #T: -1
            S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:15, T: float, TK: Float, #T: -1
                S: , K: CursorKind.FLOATING_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:15, T: double, TK: Double, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:16, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:16, T: int, TK: Int, #T: -1

*/
