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
    std::cout << "FuncDef Sem..." << std::endl;
    header->sem();
    local_def_list->sem();
    block->sem();

    std::cout << "before close scope" << std::endl;
    closeScope();
    std::cout << "after close scope" << std::endl;
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