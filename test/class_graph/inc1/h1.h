// #include <cstdint>
// #include <string>
#include <vector>
// #include "h2.h"


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


namespace NAME_SPACE1 {
namespace NAME_SPACE2 {

class Base0
{
};

std::vector<Base0, int> my_var;

class D : public std::vector<Base0>
{
};

struct Base1 : public Base0
{
};

struct Derive1 : public Base1
{
};

class Derive3 : public Base1, public Base2
{
};

using namespace NAME_SPACE4::NAME_SPACE3;
class TDerive1 : public TBase2<int, float>
{
};

class TDerive2 : public NAME_SPACE4::NAME_SPACE3::TBase2<int, float>
{
};

} // NAME_SPACE2
using namespace NAME_SPACE2;
using namespace std;

template<class T>
class TBase1
{
};

class TDerive3 : public TBase1<NAME_SPACE4::NAME_SPACE3::TBase2<NAME_SPACE4::NAME_SPACE3::TBase2<int, float>, Derive2>>
{
};

class TDerive4 : public NAME_SPACE4::NAME_SPACE3::TBase2<Derive2, Derive2>
{
};

typedef NAME_SPACE4::NAME_SPACE3::TBase2<Derive22, Derive22> TDerive5;

class TDerive6 : public TDerive5
{
};

class TDerive7 : public TBase2<TBase1<int>, int>
{
};

class Derive8 : public TDerive7
{
};

class DiamondRight : public Base1
{
};
class DiamondLeft : public NAME_SPACE2::Base1
{
};
class DiamondDerive : public DiamondRight, public DiamondLeft
{
};

} // NAME_SPACE1
