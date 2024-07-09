#include "funcdecl.hpp"

FuncDecl::FuncDecl(Header *h) : header(h) {}
FuncDecl::~FuncDecl() { delete header; }
void FuncDecl::printOn(std::ostream &out) const
{
    out << "FuncDecl(" << *header << ")";
}
void FuncDecl::sem()
{
    // FuncDecl is used for forward declaring functions.
    // The Header class is used for both forward declarations and regular function definitions.
    // Setting forward_declaration to true specifies that the Header represents a forward declared function.
    header->setForwardDeclaration();
    header->sem();

    returnedFunction.pop_back();
    returnType.pop_back();
    closeScope();
}

llvm::Value* FuncDecl::compile()
{
    header->compile();
    return nullptr;
}

void FuncDecl::setOuterFunction(std::string outer_func_name)
{
    OuterFunction[header->getHMangledName()] = outer_func_name;
}