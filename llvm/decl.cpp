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
    for (Id *id : idlist->getIds())
    {
        newVariable(id->getName(), type);
    }
    parser_type->sem();
}

llvm::Value *Decl::compile() const
{
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvmType *llvm_type = parser_type->getLLVMType(type);

    for (Id *id : idlist->getIds())
    {
        id->compile();
        llvm::StringRef varName = id->getName();
        // Create an allocation in the entry block
        llvm::AllocaInst *allocaInst = CreateEntryBlockAlloca(TheFunction, varName, llvm_type);
        NamedValues[id->getName()] = allocaInst;
    }

    return nullptr;
}
