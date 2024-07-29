
// Base class
class A {
public:
    // Virtual function
    virtual void show() {
    }
};

// Derived class B inheriting from A
class B : public A {
public:
    virtual void show() {
    }
};

// Derived class C inheriting from A
class C :  public A {
public:
    virtual void show() {
    }
};

// Derived class D inheriting from B and C
class D : public B, public C {
public:
    // Overriding the virtual function from A
    void show() override {
    }
};

int main() {
    D dObj;
    B* aPtr = &dObj;

    // Virtual function, binded at runtime
    aPtr->show();

    return 0;
}


/*

==========================
S: A, D: A, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:3, T: A, TK: Record, #T: -1, vir: False
    S: , D: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: A,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:4, T: , TK: Invalid, #T: -1, vir: False
    S: show, D: show(), K: CursorKind.CXX_METHOD, PS: A,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:6, T: void (), TK: FunctionProto, #T: -1, vir: True
        S: , D: , K: CursorKind.COMPOUND_STMT, PS: show,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:6, T: , TK: Invalid, #T: -1, vir: False
==========================
S: B, D: B, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:11, T: B, TK: Record, #T: -1, vir: False
    S: A, D: A, K: CursorKind.CXX_BASE_SPECIFIER, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:11, T: A, TK: Elaborated, #T: -1, vir: False
        S: class A, D: class A, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:11, T: A, TK: Record, #T: -1, vir: False
==========================
S: C, D: C, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:15, T: C, TK: Record, #T: -1, vir: False
    S: A, D: A, K: CursorKind.CXX_BASE_SPECIFIER, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:15, T: A, TK: Elaborated, #T: -1, vir: False
        S: class A, D: class A, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:15, T: A, TK: Record, #T: -1, vir: False
==========================
S: D, D: D, K: CursorKind.CLASS_DECL, PS: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:19, T: D, TK: Record, #T: -1, vir: False
    S: B, D: B, K: CursorKind.CXX_BASE_SPECIFIER, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:19, T: B, TK: Elaborated, #T: -1, vir: False
        S: class B, D: class B, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:19, T: B, TK: Record, #T: -1, vir: False
    S: C, D: C, K: CursorKind.CXX_BASE_SPECIFIER, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:19, T: C, TK: Elaborated, #T: -1, vir: False
        S: class C, D: class C, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:19, T: C, TK: Record, #T: -1, vir: False
    S: , D: , K: CursorKind.CXX_ACCESS_SPEC_DECL, PS: D,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:20, T: , TK: Invalid, #T: -1, vir: False
    S: show, D: show(), K: CursorKind.CXX_METHOD, PS: D,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:22, T: void (), TK: FunctionProto, #T: -1, vir: True
        S: , D: , K: CursorKind.CXX_OVERRIDE_ATTR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:22, T: , TK: Invalid, #T: -1, vir: False
        S: , D: , K: CursorKind.COMPOUND_STMT, PS: show,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:22, T: , TK: Invalid, #T: -1, vir: False
==========================
S: main, D: main(), K: CursorKind.FUNCTION_DECL, PS: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:26, T: int (), TK: FunctionProto, #T: -1, vir: False
    S: , D: , K: CursorKind.COMPOUND_STMT, PS: main,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:26, T: , TK: Invalid, #T: -1, vir: False
        S: , D: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:27, T: , TK: Invalid, #T: -1, vir: False
            S: dObj, D: dObj, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:27, T: D, TK: Elaborated, #T: -1, vir: False
                S: class D, D: class D, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:27, T: D, TK: Record, #T: -1, vir: False
                S: D, D: D, K: CursorKind.CALL_EXPR, PS: dObj,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:27, T: D, TK: Elaborated, #T: -1, vir: False
        S: , D: , K: CursorKind.DECL_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: , TK: Invalid, #T: -1, vir: False
            S: aPtr, D: aPtr, K: CursorKind.VAR_DECL, PS: main,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: C *, TK: Pointer, #T: -1, vir: False
                S: class C, D: class C, K: CursorKind.TYPE_REF, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: C, TK: Record, #T: -1, vir: False
                S: , D: , K: CursorKind.UNEXPOSED_EXPR, PS: aPtr,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: C *, TK: Pointer, #T: -1, vir: False
                    S: , D: , K: CursorKind.UNARY_OPERATOR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: D *, TK: Pointer, #T: -1, vir: False
                        S: dObj, D: dObj, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:28, T: D, TK: Elaborated, #T: -1, vir: False
        S: show, D: show, K: CursorKind.CALL_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:31, T: void, TK: Void, #T: -1, vir: False
            S: show, D: show, K: CursorKind.MEMBER_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:31, T: <bound member function type>, TK: Unexposed, #T: -1, vir: False
                S: aPtr, D: aPtr, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:31, T: A *, TK: Pointer, #T: -1, vir: False
                    S: aPtr, D: aPtr, K: CursorKind.UNEXPOSED_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:31, T: C *, TK: Pointer, #T: -1, vir: False
                        S: aPtr, D: aPtr, K: CursorKind.DECL_REF_EXPR, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:31, T: C *, TK: Pointer, #T: -1, vir: False
        S: , D: , K: CursorKind.RETURN_STMT, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:33, T: , TK: Invalid, #T: -1, vir: False
            S: , D: , K: CursorKind.INTEGER_LITERAL, PS: ,L: /home/user/programming/cpp-util/test/call_graph_virtual_func/src/virtual.cpp:33, T: int, TK: Int, #T: -1, vir: False

*/
