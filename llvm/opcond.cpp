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

    llvm::Value *r = right->compile();

    if (left != nullptr)
    {
        llvm::Value *l = left->compile();

        if (!l || !r)
            return nullptr;

        if (op == "and")
            return Builder.CreateAnd(l, r, "andtemp");
        else if (op == "or")
            return Builder.CreateOr(l, r, "ortemp");
        else
            return nullptr;
    }
    else
    {
        if (!r)
            return nullptr;

        if (op == "not")
            return Builder.CreateNot(r, "nottemp");
    }
}