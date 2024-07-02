
class Base2
{
};


namespace NAME_SPACE4 {
namespace NAME_SPACE3 {
template <class T, class U>
class TBase2
{
};
}
}

class Derive2 : public Base2
{
};

class Derive22 : public Derive2
{
};

class Standalone
{};
