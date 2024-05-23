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
        SymbolEntry *var = newVariable(id->getName(), type);
        id->setScope(var->scopeId);
    }
    parser_type->sem();
}

llvm::Value *Decl::compile()
{
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvmType *llvm_type = getLLVMType(type, TheContext);

    for (Id *id : idlist->getIds())
    {
        // Use mangled name for llvm variable instead of real name to consider different scopes.
        std::string mangled_name = id->getMangledName();

        // Create an allocation in the entry block
        llvm::AllocaInst *allocaInst = CreateEntryBlockAlloca(TheFunction, llvm::StringRef(mangled_name), llvm_type);
        NamedValues[mangled_name] = allocaInst;
    }
    return nullptr;
}
