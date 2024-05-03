#include "binop.hpp"

BinOp::BinOp(Expr *l, char *s, Expr *r) : left(l), op(s), right(r) {}

BinOp::~BinOp()
{
    delete left;
    delete right;
}

void BinOp::printOn(std::ostream &out) const
{
    out << op << "(" << *left << ", " << *right << ")";
}

void BinOp::sem()
{
    std::cout << "BinOp Sem..." << std::endl;    
    left->type_check(typeInteger);
    right->type_check(typeInteger);
    type = typeInteger;
}
