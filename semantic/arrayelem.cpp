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
    std::cout << "ArrayElem Sem..." << std::endl;  
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for(Expr *e : expr_list) {
        e->type_check(typeInteger);
        e->sem();
    }
}