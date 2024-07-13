#include "funcdef.hpp"

FuncDef::FuncDef(Header *h, LocalDefList *ldl, Block *b) : header(h), local_def_list(ldl), block(b) {}
FuncDef::~FuncDef()
{
    delete header;
    delete local_def_list;
    delete block;
}
void FuncDef::printOn(std::ostream &out) const
{
    out << "FuncDef(";
    out << *header << " " << *local_def_list << " " << *block;
    out << ")";
}
void FuncDef::sem()
{
    header->sem();
    local_def_list->sem();
    block->sem();

    lookupEntry(header->getId()->getName(), LOOKUP_ALL_SCOPES, true);

    if(!equalType(header->getReturnType(), typeVoid) && !returnedFunction.back()) {
        std::string func_name = header->getId()->getName();
        std::string msg = "Non-void function " + func_name + " has no return statement.";
        SemanticError(msg.c_str());
    }

    returnedFunction.pop_back();
    returnType.pop_back();
    closeScope();
}

void FuncDef::ProgramSem()
{
    /* Program should:
        *      1) NOT take parameters
        *     2) Return nothing, i.e. have type typeVoid
        */ 

    if(header->getFParamList() != nullptr)
        SemanticError("Program cannot take parameters.");

    if(!equalType(header->getReturnType(), typeVoid))
        SemanticError("Program should have type void.");

    sem();
}