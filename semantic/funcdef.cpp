class LocalDef;

#include "ast.hpp"
#include "localdef.hpp"
#include "funcdef.hpp"

// FuncDef::FuncDef(Header *h, LocalDef *ld, Stmt *s) : header(h), localdef(ld), stmt(s) {}
FuncDef::FuncDef(Header *h, LocalDef *ld, Block *b) : header(h), localdef(ld), block(b) {}
FuncDef::~FuncDef()
{
    delete header;
    delete localdef;
    delete block;
    // delete stmt;
}
void FuncDef::printOn(std::ostream &out) const
{
    out << "FuncDef(";
    out << *header << " " << *localdef << " " << *block;
    out << ")";
}
void FuncDef::sem()
{
    // might need to open scope (scope should end after the end of the block)

    header->sem(); // not forward declared
    localdef->sem();
    // stmt->sem();
    block->sem();
}