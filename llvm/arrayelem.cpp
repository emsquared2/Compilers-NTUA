#include "arrayelem.hpp"

ArrayElem::ArrayElem(LValue *l, ExprList *e) : left(l), exprlist(e) {}
ArrayElem::~ArrayElem()
{
    delete left;
    delete exprlist;
}
void ArrayElem::printOn(std::ostream &out) const
{
    std::vector<Expr *> exprs = exprlist->getExprList();
    out << "ArrayElem(" << *left;
    for (auto e = exprs.rbegin(); e != exprs.rend(); ++e)
    {
        out << "[" << **e << "]";
    }
    out << ")";
}
std::string ArrayElem::getArrayElem()
{
    // return left->getName() + "[" + std::to_string(expr->eval()) + "]";
}
void ArrayElem::sem()
{
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for (Expr *e : expr_list)
    {
        e->type_check(typeInteger);
        e->sem();
    }

    Type t = left->getType();
    if (t->refType == nullptr)
    {
        std::string name = left->getName();
        std::string msg = "Variable " + name + " is not of type Array";
        SemanticError(msg.c_str());
    }

    type = findArrayType(t);
}

llvm::Value *ArrayElem::compile_ptr() const
{
    std::string name = left->getName();

    // Look this variable up in the function.
    llvm::AllocaInst *A = NamedValues[name + '_' + std::to_string(scope) + '_'];
    if (!A)
    {
        std::string msg = "Id: Unknown variable name: " + name + '_' + std::to_string(scope) + '_' + ".";
        return LogErrorV(msg.c_str());
    }

    std::string mangled_name = name + '_' + std::to_string(scope) + '_';
    // // Load the value.
    return Builder.CreateLoad(llvm::PointerType::get(A->getAllocatedType(), 0), A, mangled_name.c_str());

    // return A;
}

llvm::Value *ArrayElem::compile() const
{
    std::string name = left->getName();

    // Look this variable up in the function.
    llvm::AllocaInst *A = NamedValues[name + '_' + std::to_string(scope) + '_'];
    if (!A)
    {
        std::string msg = "Id: Unknown variable name: " + name + '_' + std::to_string(scope) + ".";
        return LogErrorV(msg.c_str());
    }

    std::string mangled_name = name + '_' + std::to_string(scope) + '_';
    // // Load the value.
    return Builder.CreateLoad(A->getAllocatedType(), A, mangled_name.c_str());

    // return A;
}
