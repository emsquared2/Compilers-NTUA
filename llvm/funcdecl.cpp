#include "funcdecl.hpp"

FuncDecl::FuncDecl(Header *h) : header(h) {}
FuncDecl::~FuncDecl() { delete header; }
void FuncDecl::printOn(std::ostream &out) const
{
    out << "FuncDecl(" << *header << ")";
}
void FuncDecl::sem()
{
    // FuncDecl is for forward declaring function.
    // Header class is for both forward and non forward functions.
    // Set forward_declaration to true to specify a header that represents a forward declared function.
    header->set_forward_declaration();
    header->sem();

    returnedFunction.pop_back();
    closeScope();
}

llvm::Value* FuncDecl::compile() const
{
    header->compile();
    return nullptr;
}