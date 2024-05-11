#include "header.hpp"

Header::Header(Id *id, RetType *t, FParamList *fpl = nullptr) : id(id), ret_type(t), fparamlist(fpl)
{
    type = ret_type->ConvertToType();
}

Header::~Header()
{
    delete id;
    if (fparamlist != nullptr)
    {
        delete fparamlist;
    }
    delete ret_type;
    if (type)
    {
        destroyType(type);
    }
}
void Header::printOn(std::ostream &out) const
{
    out << "Header(fun " << *id << "(";
    if (fparamlist != nullptr)
        out << *fparamlist;
    out << ") : ";
    out << *ret_type;
    // if(type) printType(type);
    out << ")";
}

void Header::set_forward_declaration()
{
    forward_declaration = true;
}

FParamList *Header::getFParamList()
{
    return fparamlist;
}

Type Header::getReturnType()
{
    return type;
}
Id *Header::getId()
{
    return id;
}
void Header::sem()
{
    /* Check if id exists in symbol table. If it exists, throw error.
     * If not:
     *  1) add it to the symbol table
     *  2) open a new scope
     *  3) store the return type
     *  4) paramlit->sem()
     *  5) close the scope?
     */

    SymbolEntry *function = newFunction(id->getName());

    // add case when parser identifies forward declaration
    if (forward_declaration)
        forwardFunction(function);

    openScope();
    returnedFunction.push_back(false);
    /*
    Here we have to add all the parameters to the symbol entry.
    The SymbolEntry of the function (look *function above) is also needed when creating a new function parameter.
    We have 2 options:
    Option 1: Pass the function SymbolEntry to the ParamList and handle the sem() function inside the ParamList class
    Option 2: Add a method to the ParamList class that returns the private vector of Parameters. Then handle each Param in this sem() function.

    Option 1 might be easier to implement but having a field of SymbolEntry on a Class might seem a little be counterintuitive.

    Option 2 on the other hand might be a cleaner option as the SymbolEntry is abstracted from the classes.
    */
    if (fparamlist != nullptr)
    {
        fparamlist->setSymbolEntry(function);
        fparamlist->sem();
    }

    endFunctionHeader(function, type);
}

std::vector<llvmType *> Header::getLLVM_param_types()
{
    return (fparamlist) ? fparamlist->getLLVM_params() : std::vector<llvmType *> {}; 
}

std::vector<llvm::StringRef> Header::getLLVM_param_names()
{
    return (fparamlist) ? fparamlist->getLLVM_param_names() : std::vector<llvm::StringRef> {};
}

llvm::Function *Header::compile() const
{
    // fparamlist->compile();
    llvm::Function *function = TheModule->getFunction(id->getName());
    if (!function)
    {
        llvmType *return_type = ret_type->getLLVMType(type);
        std::vector<llvmType *> llvm_param_types = (fparamlist) ? fparamlist->getLLVM_params() : std::vector<llvmType *>{};
        llvm::FunctionType *funcType = llvm::FunctionType::get(return_type, llvm_param_types, false);
        function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, id->getName(), TheModule.get());
    }

    return function;
}
