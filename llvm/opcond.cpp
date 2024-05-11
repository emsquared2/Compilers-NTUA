#include "opcond.hpp"

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

llvm::Value *OpCond::compile() const
{

    llvm::Value *R = right->compile();

    if (left != nullptr)
    {
        llvm::Value *L = left->compile();

        if (!L || !R)
            return nullptr;

        if (op == "and")
            return Builder.CreateAnd(L, R, "andtemp");
        else if (op == "or")
            return Builder.CreateOr(L, R, "ortemp");
        else
            return LogErrorV("invalid binary operator");
    }
    else
    {
        if (!R)
            return nullptr;

        if (op == "not")
            return Builder.CreateNot(R, "nottemp");
        else
            return LogErrorV("invalid binary operator");
    }
}