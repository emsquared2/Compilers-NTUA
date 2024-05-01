#include "conditions.hpp"

/* ---------------------------------------------------------------------
   ------------------------------ OpCond -------------------------------
   --------------------------------------------------------------------- */

OpCond::OpCond(Cond *l, char *s, Cond *r) : left(l), op(s), right(r) {}

OpCond::OpCond(char *s, Cond *r) : left(), op(s), right(r) {}

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

/* ---------------------------------------------------------------------
   ----------------------------- CompCond ------------------------------
   --------------------------------------------------------------------- */

CompCond::CompCond(Expr *l, char *s, Expr *r) : left(l), op(s), right(r) {}

CompCond::~CompCond()
{
    delete left;
    delete right;
}

void CompCond::printOn(std::ostream &out) const
{
    out << op << "(" << *left << ", " << *right << ")";
}

void CompCond::sem()
{
    left->type_check(typeInteger);
    right->type_check(typeInteger);

    type = typeBoolean;
}