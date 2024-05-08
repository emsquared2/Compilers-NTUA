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

    switch (op) {
      case "+": return Builder.CreateAdd(l, r, "addtmp");
      case "-": return Builder.CreateSub(l, r, "subtmp");
      case "*": return Builder.CreateMul(l, r, "multmp");
      case "/": return Builder.CreateSDiv(l, r, "divtmp");
      case "%": return Builder.CreateSRem(l, r, "modtmp");
      default: return nullptr;
    }
    return nullptr;
}