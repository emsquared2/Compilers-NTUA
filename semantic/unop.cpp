#include "unop.hpp"

UnOp::UnOp(const char *s, Expr *e) : op(s), right(e) {}

UnOp::~UnOp()
{
    delete right;
}

void UnOp::printOn(std::ostream &out) const
{
    out << op << "(" << *right << ")";
}

void UnOp::sem()
{
    right->type_check(typeInteger);
    type = typeInteger;
}