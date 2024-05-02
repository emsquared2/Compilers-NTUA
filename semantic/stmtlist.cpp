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
    for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **s;
    }
    out << ")";
}
void StmtList::sem()
{
    std::cout << "StmtList Sem..." << std::endl;
    for (Stmt *s : stmt_list)
        s->sem();
}