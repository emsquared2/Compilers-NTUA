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

llvm::Value* UnOp::compile()
{
    llvm::Value* R = right->compile();

    if(op == "+")
        return R;
    else if (op == "-")
        return Builder.CreateNeg(R, "unarytemp");
    else
        return nullptr;
}
