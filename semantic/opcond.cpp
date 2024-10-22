#include "opcond.hpp"

OpCond::OpCond(Cond *l, const char *s, Cond *r) : left(l), op(s), right(r) {}

OpCond::OpCond(const char *s, Cond *r) : left(), op(s), right(r) {}

OpCond::~OpCond()
{
    if (left != nullptr)
    {
        delete left;
    }
    delete right;
}

void OpCond::printOn(std::ostream &out) const
{
    out << op << "(";
    if (left != nullptr)
        out << *left << ", ";
    out << *right << ")";
}

void OpCond::sem()
{
    if (left != nullptr)
        left->type_check(typeBoolean);
    right->type_check(typeBoolean);
    
    type = typeBoolean;
}