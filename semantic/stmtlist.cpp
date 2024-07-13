#include "stmtlist.hpp"


StmtList::StmtList() : stmt_list() {}
StmtList::~StmtList()
{
    for (Stmt *stmt : stmt_list)
    {
        delete stmt;
    }
}
void StmtList::append(Stmt *s) { stmt_list.push_back(s); }
void StmtList::printOn(std::ostream &out) const
{
    out << "StmtList(";
    bool first = true;
    for (Stmt *s : getReversed(stmt_list))
    {
        if (!first)
            out << ", ";
        first = false;
        out << *s;
    }
    out << ")";
}
void StmtList::sem()
{
    for (Stmt *s : getReversed(stmt_list))
        s->sem();
}