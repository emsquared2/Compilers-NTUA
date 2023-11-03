class LocalDef;

#include "ast.hpp"
#include "localdef.hpp"
#include "funcdef.hpp"

FuncDef::FuncDef(Header *h, LocalDef *ld, Stmt *s) : header(h), localdef(ld), stmt(s) {}
FuncDef::~FuncDef()
{
    delete header;
    delete localdef;
    delete stmt;
}
void FuncDef::printOn(std::ostream &out) const
{
    out << "FuncDef(";
    out << *header << " " << *localdef << " " << *stmt;
    out << ")";
}
