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
    llvm::Value *L = left->compile();
    llvm::Value *R = right->compile();

    if(!L || !R)
        return nullptr;

    if (op == "+")
        return Builder.CreateAdd(L, R, "addtmp");
    else if (op == "-")
        return Builder.CreateSub(L, R, "subtmp");
    else if (op == "*")
        return Builder.CreateMul(L, R, "multmp");
    else if (op == "div")
        return Builder.CreateSDiv(L, R, "divtmp");
    else if (op == "mod")
        return Builder.CreateSRem(L, R, "modtmp");
    else
        return nullptr;
}