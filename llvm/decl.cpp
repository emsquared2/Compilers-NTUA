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
    for (Id *id : idlist->get_idlist())
    {
        newVariable(id->getName(), type);
    }
    parser_type->sem();
}
llvm::Value * Decl::compile() const
{
    llvmType *llvm_type = parser_type->getLLVMType(type);

    for (Id *id : idlist->get_idlist()) {
        // Create an allocation in the entry block
        llvm::Value *allocaInst = Builder.CreateAlloca(llvm_type, nullptr, id->getName());
        // Optionally initialize or store other metadata
        // var[id->getName()] = allocaInst; /* var --> possibly a map<string, llvm:Value* */
    }

    return nullptr;
}