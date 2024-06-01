#include "binop.hpp"

BinOp::BinOp(Expr *l, const char *s, Expr *r) : left(l), op(s), right(r) {}

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


llvm::Value* BinOp::compile()
{
    llvm::Value *L = left->compile();
    llvm::Value *R = right->compile();

    if(!L || !R)
        return nullptr;

    std::string str_op = std::string(op);
    if (str_op == "+")
        return Builder.CreateAdd(L, R, "addtmp");
    else if (str_op == "-")
        return Builder.CreateSub(L, R, "subtmp");
    else if (str_op == "*")
        return Builder.CreateMul(L, R, "multmp");
    else if (str_op == "div")
        return Builder.CreateSDiv(L, R, "divtmp");
    else if (str_op == "mod")
        return Builder.CreateSRem(L, R, "modtmp");
    else
        return nullptr;
}