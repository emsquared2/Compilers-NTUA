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
    left->type_check(typeInteger);
    right->type_check(typeInteger);
    type = typeInteger;
}


llvm::Value* BinOp::compile() const
{
    llvm::Value *l = left->compile();
    llvm::Value *r = right->compile();

    if(!l or !r)
        return nullptr;

    if (op == "+")
        return Builder.CreateAdd(l, r, "addtmp");
    else if (op == "-")
        return Builder.CreateSub(l, r, "subtmp");
    else if (op == "*")
        return Builder.CreateMul(l, r, "multmp");
    else if (op == "div")
        return Builder.CreateSDiv(l, r, "divtmp");
    else if (op == "mod")
        return Builder.CreateSRem(l, r, "modtmp");
    else
        return nullptr;
}