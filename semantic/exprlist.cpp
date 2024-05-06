#include "exprlist.hpp"

ExprList::ExprList() : expr_list() {}
ExprList::~ExprList()
{
    for (Expr *expr : expr_list)
    {
        delete expr;
    }
}
void ExprList::append(Expr *e) { expr_list.push_back(e); }
void ExprList::printOn(std::ostream &out) const
{
    out << "ExprList(";
    bool first = true;
    // for (Expr *e : expr_list)
    for (auto e = expr_list.rbegin(); e != expr_list.rend(); ++e)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **e;
    }
    out << ")";
}
std::vector<Expr *> ExprList::getExprList()
{
    return expr_list;
}
bool ExprList::isEmpty()
{
    return expr_list.empty();
}
void ExprList::sem()
{
    for (Expr *e : expr_list)
        e->sem();
}
