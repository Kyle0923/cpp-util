template<typename T>
class GrandParent
{
};

template<typename T>
class Parent : public GrandParent<T> {};

class VerySimple {};
class Simple : public VerySimple {};

class Child : public Parent<int>, public Simple
{
};
