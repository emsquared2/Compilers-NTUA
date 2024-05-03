#include "callstmt.hpp"


CallStmt::CallStmt(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
CallStmt::~CallStmt()
{
    delete id;
    delete expr_list;
}
void CallStmt::printOn(std::ostream &out) const
{
    out << "CallStmt(" << *id << "(";
    if (expr_list != nullptr)
        out << *expr_list;
    out << "))";
}

void CallStmt::sem()
{
    std::cout << "CallStmt Sem..." << std::endl;
    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);
    // TODO: Check if function is called properly (number and type of arguments)
}