class FuncDef;

#include "ast.hpp"
#include "funcdef.hpp"
#include "localdef.hpp"

LocalDef::LocalDef(FuncDef *fdef) : funcdef(fdef), funcdecl(nullptr), decl(nullptr) {}
LocalDef::LocalDef(FuncDecl *fdecl) : funcdef(nullptr), funcdecl(fdecl), decl(nullptr) {}
LocalDef::LocalDef(Decl *decl) : funcdef(nullptr), funcdecl(nullptr), decl(decl) {}

LocalDef::~LocalDef()
{
    delete funcdef;
    delete funcdecl;
    delete decl;
}
void LocalDef::printOn(std::ostream &out) const
{
    out << "LocalDef(";
    if (funcdef != nullptr)
    {
        out << *funcdef;
    }
    else if (funcdecl != nullptr)
    {
        out << *funcdecl;
    }
    else if (decl != nullptr)
    {
        out << *decl;
    }
    out << ")";
}