#include "unop.hpp"

UnOp::UnOp(char *s, Expr *e) : op(s), right(e) {}

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

llvm::Value* UnOp::compile() const
{
    llvm::Value* r = right->compile();

    if(op == "+")
        return r;
    else if (op == "-")
        return Builder.CreateNeg(r, "unarytemp");
    else
        return nullptr;
}
