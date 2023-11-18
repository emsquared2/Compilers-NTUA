#include "ast.hpp"
#include "types.hpp"

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

Array::Array(Integer *i) : integer(i), character(nullptr) {}
Array::Array(Character *t) : integer(nullptr), character(t) {}
Array::~Array()
{
    delete integer;
    delete character;
}
void Array::printOn(std::ostream &out) const
{
    if (integer != nullptr)
    {
        out << *integer;
    }
    else
    {
        out << *character;
    }
    out << " array";
}
Nothing::Nothing() {}
void Nothing::printOn(std::ostream &out) const
{
    out << "nothing";
}