#include "compcond.hpp"

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
    left->sem();
    right->sem();

    Type left_type = left->getType();
    Type right_type = right->getType();

    // Check operands' type (typeInteher or typeChar)
    if (!equalType(left_type, typeInteger) && !equalType(left_type, typeChar))
        SemanticError("CompCond: Left operand should be typeInteger or typeChar");

    if (!equalType(right_type, typeInteger) && !equalType(right_type, typeChar))
        SemanticError("CompCond: Right operand should be typeInteger or typeChar");

    // Check if right and left operand have the same type
    if (!equalType(left_type, right_type))
        SemanticError("CompCond: Operands should have the same type.");

    type = typeBoolean;
}

llvm::Value *CompCond::compile() const
{
    llvm::Value *L = left->compile();
    llvm::Value *R = right->compile();

    if (!L || !R)
        return nullptr;

    if (op == "=")
        return Builder.CreateICmpEQ(L, R, "eqtmp");
    else if (op == "#")
        return Builder.CreateICmpNE(L, R, "netmp");
    else if (op == "<")
        return Builder.CreateICmpSLT(L, R, "lttmp");
    else if (op == ">")
        return Builder.CreateICmpSGT(L, R, "gttmp");
    else if (op == "<=")
        return Builder.CreateICmpSLE(L, R, "letmp");
    else if (op == ">=")
        return Builder.CreateICmpSGE(L, R, "getmp");
    else
        return LogErrorV("invalid binary operator");
}