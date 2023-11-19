#include "ast.hpp"
#include "types.hpp"

inline std::ostream &operator<<(std::ostream &out, const Types &t)
{
    t.printOn(out);
    return out;
}

Integer::Integer() {}
void Integer::printOn(std::ostream &out) const
{
    out << "int";
}

Boolean::Boolean() {}
void Boolean::printOn(std::ostream &out) const
{
    out << "bool";
}

Character::Character() {}
void Character::printOn(std::ostream &out) const
{
    out << "char";
}

Str::Str() {}
void Str::printOn(std::ostream &out) const
{
    out << "str";
}

Array:: Array(Types *t, ArrayDim *d) : type(t), dim(d) {}
Array::~Array()
{
    delete type;
    delete dim;
}
void Array::printOn(std::ostream &out) const
{
    out << *type << " array" << *dim;
}

Nothing::Nothing() {}
void Nothing::printOn(std::ostream &out) const
{
    out << "nothing";
}