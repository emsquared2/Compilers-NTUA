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

void ArrayElem::sem()
{
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for(Expr *e : expr_list) {
        e->type_check(typeInteger);
        e->sem();
    }

    Type t = left->getType();
    if(t->refType == nullptr) {
        std::string name = left->getName();
        std::string msg = "Variable " + name + " is not of type Array";
        SemanticError(msg.c_str());
    }
    
    type = findArrayType(t, exprlist->getExprList().size());
}