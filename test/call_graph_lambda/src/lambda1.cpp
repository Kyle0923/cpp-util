#include <functional>
// #include "header.hpp"

int forward_call( std::function<int(void)> func) {
    return func();
}

int main() {
    forward_call(
        []() {return 0;}
    );
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
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:8, T: int (), TK: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:8, T: , TK: Invalid, #T: -1
        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:9, T: int, TK: Int, #T: -1
            S: forward_call, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:9, T: int, TK: Int, #T: -1
                S: forward_call, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:9, T: int (*)(std::function<int ()>), TK: Pointer, #T: -1
                    S: forward_call, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:9, T: int (std::function<int ()>), TK: FunctionProto, #T: -1
                S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                    S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                            S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                                S: function, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                                    S: function, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: std::function<int ()>, TK: Elaborated, #T: 1
                                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10:9), TK: Record, #T: -1
                                            S: , K: CursorKind.LAMBDA_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: (lambda at /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10:9), TK: Record, #T: -1
                                                S: , K: CursorKind.COMPOUND_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: , TK: Invalid, #T: -1
                                                    S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: , TK: Invalid, #T: -1
                                                        S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:10, T: int, TK: Int, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: , TK: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_lambda/src/lambda1.cpp:12, T: int, TK: Int, #T: -1


*/
