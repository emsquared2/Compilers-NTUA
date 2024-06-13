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
        // id->setScope(var->scopeId);
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
        // Create an allocation in the entry block
        llvm::AllocaInst *allocaInst = CreateEntryBlockAlloca(TheFunction, llvm::StringRef(mangled_names[i]), llvm_type);
        NamedValues[mangled_names[i]] = allocaInst;
    }
    return nullptr;
}

/*
 * This function adds captured variables to the function's parameter lists if they are used in an inner function.
 * It ensures that the innfer function's parameter list includes pointers to these captured variables so that they can be accessed
 * within the inner function. For example:
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
 * - Variable x is a variable defined in `outer` and is captured by `inner`, so var x needs to be included in `inner`'s parameter list.
 */
void Decl::addCapturedParameters(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref)
{
    llvmType *t = llvm::PointerType::get(getLLVMType(type, TheContext), 0);

    for (auto &mangled_name : mangled_names) 
    {
        if(CapturedVariables.find(mangled_name) != CapturedVariables.end())
        {
            param_names->push_back(mangled_name);
            param_types->push_back(t);
            CapturedVariableOffset[mangled_name] = param_names->size() - 1;
            ref->push_back(false);
        }
    }
}
