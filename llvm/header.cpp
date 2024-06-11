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
    SymbolEntry *function = newFunction(id->getName());

    mangled_name = getMangledName(id->getName(), function->scopeId);
    FunctionDepth[mangled_name] = function->nestingLevel;

    // add case when parser identifies forward declaration
    if (forward_declaration)
        forwardFunction(function);

    openScope();
    returnedFunction.push_back(false);

    if (fparamlist != nullptr)
    {
        fparamlist->setSymbolEntry(function);
        fparamlist->sem();
    }

    endFunctionHeader(function, type);
}

llvm::Function *Header::compile()
{
    if (fparamlist)
        fparamlist->compile(&llvm_param_names, &llvm_param_types);

    llvm::Function *function = TheModule->getFunction(mangled_name);
    if (!function)
    {
        llvmType *return_type = getLLVMType(type, TheContext);

        llvm::FunctionType *funcType = llvm::FunctionType::get(return_type, (fparamlist) ? llvm_param_types : std::vector<llvmType *>{}, false);
        function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, mangled_name, TheModule.get());
    }

    // std::cout << "Depth of " << mangled_name << " : " << FunctionDepth[mangled_name] << std::endl;
    return function;
}

std::string Header::getHMangledName()
{
    return mangled_name;
}

std::vector<llvmType *> Header::getLLVM_param_types()
{
    return (fparamlist) ? llvm_param_types : std::vector<llvmType *>{};
    return llvm_param_types;
}

std::vector<std::string> Header::getLLVM_param_names()
{
    return (fparamlist) ? llvm_param_names : std::vector<std::string>{};
}