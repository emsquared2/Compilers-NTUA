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
        destroyType(type);
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
        mangled_names.push_back(getMangledName(id->getName(), var->scopeId));
    }
    parser_type->sem();
}

llvm::Value *Decl::compile()
{
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvmType *llvm_type = getLLVMType(type, TheContext);

    for (int i = 0; i < idlist->getIds().size(); ++i)
    {
        // Create an allocation in the entry block for each variable.
        llvm::AllocaInst *allocaInst = CreateEntryBlockAlloca(TheFunction, llvm::StringRef(mangled_names[i]), llvm_type);
        NamedValues[mangled_names[i]] = allocaInst;
    }
    return nullptr;
}

/*
 * This function identifies and adds captured variables to the parameter list of an inner function.
 * Captured variables are those defined in an outer function but used in an inner function.
 * Pointers to these variables are later added to the stack frame struct of the corresponding function.
 * It ensures that the inner function has pointers to these captured variables, enabling their access within the inner function.
 * Example:
 * fun outer() : nothing
 *      var x : int;
 *      fun inner() : nothing
 *      {
 *          x <- x + 1;
 *      }
 * {
 *      x <- 10;
 *      inner();
 * }
 * 
 * Here, variable x is defined in `outer` and used in `inner`, so `x` needs to be included in `inner`'s parameter list.
 */
void Decl::addCapturedVariables(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref)
{
    // Get the LLVM type for pointers to the declared type.
    llvmType *t = llvm::PointerType::get(getLLVMType(type, TheContext), 0);

    // Check each mangled name to see if it's a captured variable.
    for (auto &mangled_name : mangled_names) 
    {
        if(CapturedVariables.find(mangled_name) != CapturedVariables.end())
        {
            // Add the captured variable to the parameter lists.
            param_names->push_back(mangled_name);
            param_types->push_back(t);
            CapturedVariableOffset[mangled_name] = param_names->size() - 1;
            ref->push_back(false);
        }
    }
}
