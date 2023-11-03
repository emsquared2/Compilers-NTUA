class FuncDef;

#include "ast.hpp"
#include "funcdef.hpp"
#include "local.hpp"

Local::Local(FuncDef *fdef) : funcdef(fdef), funcdecl(nullptr), decl(nullptr) {}
Local::Local(FuncDecl *fdecl) : funcdef(nullptr), funcdecl(fdecl), decl(nullptr) {}
Local::Local(Decl *decl) : funcdef(nullptr), funcdecl(nullptr), decl(decl) {}

Local::~Local()
{
    delete funcdef;
    delete funcdecl;
    delete decl;
}
void Local::printOn(std::ostream &out) const
{
    out << "Local(";
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