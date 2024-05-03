#include "decl.hpp"

Decl::Decl(IdList *idl, FParType *fpt) : idlist(idl), parser_type(fpt) 
{ 
    type = parser_type->ConvertToType(); 
}

Decl::~Decl()
{
    delete idlist;
    delete parser_type;
    if (type)
    {
        destroyType(type);
    }
}

void Decl::printOn(std::ostream &out) const
{
    out << "Decl(var " << *idlist << " : " << *parser_type;
    // if(type) { printType(type); }
    out << ")";
}

void Decl::sem()
{
    std::cout << "Decl Sem..." << std::endl;
    for (Id *id : idlist->get_idlist())
    {
        newVariable(id->getName(), type);
    }
}