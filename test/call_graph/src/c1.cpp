#include "header.hpp"


int main() {
    int a = 0;
    Foo foo;
    Bar<int> barT;
    Bar<float> barF;
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
S: FOO, K: CursorKind.CLASS_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:5, T: Invalid, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:4, T: Unexposed, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:6, T: Invalid, #T: -1
    S: func1, K: CursorKind.CXX_METHOD, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: FunctionProto, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func1,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: Invalid, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: Int, #T: -1
    S: func2, K: CursorKind.CXX_METHOD, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: FunctionProto, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: func2,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Invalid, #T: -1
                S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Int, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Int, #T: -1
    S: func2, K: CursorKind.FUNCTION_TEMPLATE, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: FunctionProto, #T: -1
        S: U, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func2,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:9, T: Unexposed, #T: -1
        S: b, K: CursorKind.PARM_DECL, PS: func2,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Unexposed, #T: -1
            S: U, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:9, T: Unexposed, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func2,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Invalid, #T: -1
                S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Unexposed, #T: -1
    S: func4, K: CursorKind.CXX_METHOD, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: FunctionProto, #T: -1
        S: a, K: CursorKind.PARM_DECL, PS: func4,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
        S: b, K: CursorKind.PARM_DECL, PS: func4,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func4,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Invalid, #T: -1
                S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                    S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                        S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                    S: b, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                        S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
    S: func5, K: CursorKind.CXX_METHOD, PS: FOO,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: FunctionProto, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: func5,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Invalid, #T: -1
                S: func4, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                    S: func4, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Pointer, #T: -1
                        S: func4, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: FunctionProto, #T: -1
                    S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Int, #T: -1
                    S: func4, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                        S: func4, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Pointer, #T: -1
                            S: func4, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: FunctionProto, #T: -1
                        S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Int, #T: -1
                        S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Int, #T: -1
==========================
S: Bar, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:15, T: Record, #T: -1
    S: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: Bar,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:16, T: Invalid, #T: -1
    S: bar_func, K: CursorKind.CXX_METHOD, PS: Bar,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: FunctionProto, #T: -1
    S: bar_func2, K: CursorKind.CXX_METHOD, PS: Bar,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:18, T: FunctionProto, #T: -1
        S: , K: CursorKind.COMPOUND_STMT, PS: bar_func2,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:18, T: Invalid, #T: -1
            S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:18, T: Invalid, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:18, T: Int, #T: -1
==========================
S: bar_func, K: CursorKind.CXX_METHOD, PS: Bar,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: FunctionProto, #T: -1
    S: class Bar, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:15, T: Record, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: bar_func,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:21, T: Int, #T: -1
==========================
S: func6, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:24, T: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func6,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: Unexposed, #T: -1
    S: T, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: Unexposed, #T: -1
    S: b, K: CursorKind.PARM_DECL, PS: func6,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:24, T: Unexposed, #T: -1
        S: T, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:23, T: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func6,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:24, T: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:24, T: Invalid, #T: -1
            S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:24, T: Unexposed, #T: -1
==========================
S: func6, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: FunctionProto, #T: -1
    S: b, K: CursorKind.PARM_DECL, PS: func6,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Int, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func6,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Invalid, #T: -1
            S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Int, #T: -1
                S: b, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Int, #T: -1
                    S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:27, T: Int, #T: -1
==========================
S: func7, K: CursorKind.FUNCTION_TEMPLATE, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: FunctionProto, #T: -1
    S: T, K: CursorKind.TEMPLATE_TYPE_PARAMETER, PS: func7,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:29, T: Unexposed, #T: -1
    S: T, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:29, T: Unexposed, #T: -1
    S: b, K: CursorKind.PARM_DECL, PS: func7,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Unexposed, #T: -1
        S: T, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:29, T: Unexposed, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: func7,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Invalid, #T: -1
            S: , K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Dependent, #T: -1
                S: , K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Dependent, #T: -1
                    S: func6, K: CursorKind.OVERLOADED_DECL_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Invalid, #T: -1
                    S: T, K: CursorKind.TYPE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:29, T: Unexposed, #T: -1
                S: b, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Unexposed, #T: -1
==========================
S: main, K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: FunctionProto, #T: -1
    S: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:32, T: Invalid, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Invalid, #T: -1
            S: a, K: CursorKind.VAR_DECL, PS: main,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: a,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
        S: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Invalid, #T: -1
            S: foo, K: CursorKind.VAR_DECL, PS: main,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Unexposed, #T: 1
                S: FOO, K: CursorKind.TEMPLATE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:5, T: Invalid, #T: -1
                S: FOO, K: CursorKind.CALL_EXPR, PS: foo,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:5, T: Unexposed, #T: 1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:35, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: func1, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: Int, #T: -1
                S: func1, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:7, T: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Unexposed, #T: 1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:36, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: func2, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Int, #T: -1
                S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:8, T: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Unexposed, #T: 1
                S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:37, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: func2, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Int, #T: -1
                S: func2, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:10, T: Unexposed, #T: -1
                    S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Unexposed, #T: 1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:37, T: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:38, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: func4, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Int, #T: -1
                S: func4, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: Pointer, #T: -1
                    S: func4, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:11, T: FunctionProto, #T: -1
                        S: foo, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:34, T: Unexposed, #T: 1
                S: a, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
                    S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:38, T: Int, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: , K: CursorKind.CSTYLE_CAST_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: Int, #T: -1
                S: func7, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Int, #T: -1
                    S: func7, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Unexposed, #T: -1
                        S: func7, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Pointer, #T: -1
                            S: func7, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: FunctionProto, #T: -1
                        S: , K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: Unexposed, #T: -1
                            S: , K: CursorKind.FLOATING_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:39, T: Double, #T: -1
        S: , K: CursorKind.BINARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: Int, #T: -1
            S: a, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:33, T: Int, #T: -1
            S: func7, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Unexposed, #T: -1
                S: func7, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: Pointer, #T: -1
                    S: func7, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:30, T: FunctionProto, #T: -1
                S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:40, T: Int, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:41, T: Invalid, #T: -1
            S: func5, K: CursorKind.CALL_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Int, #T: -1
                S: func5, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: Pointer, #T: -1
                    S: func5, K: CursorKind.DECL_REF_EXPR, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:12, T: FunctionProto, #T: -1
                        S: FOO, K: CursorKind.TEMPLATE_REF, PS: ,L: DEF_LOC /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:5, T: Invalid, #T: -1
        S: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:42, T: Invalid, #T: -1
            S: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph/src/c1.cpp:42, T: Int, #T: -1

*/
