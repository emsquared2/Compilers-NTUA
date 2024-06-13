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
    if (!isTopLevel(mangled_name))
        addStaticLinkToFunctionSignature(&llvm_param_names, &llvm_param_types);

    if (fparamlist)
        fparamlist->compile(&llvm_param_names, &llvm_param_types);

    llvm::Function *function = TheModule->getFunction(mangled_name);
    if (!function)
    {
        llvmType *return_type = getLLVMType(type, TheContext);

        llvm::FunctionType *funcType = llvm::FunctionType::get(return_type, llvm_param_types, false);
        function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, mangled_name, TheModule.get());
    }
    return function;
}

std::string Header::getHMangledName()
{
    return mangled_name;
}

std::vector<llvmType *> Header::getLLVM_param_types()
{
    // return (fparamlist) ? llvm_param_types : std::vector<llvmType *>{};
    return llvm_param_types;
}

std::vector<std::string> Header::getLLVM_param_names()
{
    // return (fparamlist) ? llvm_param_names : std::vector<std::string>{};
    return llvm_param_names;
}


/*
 * This function ensures that the inner function receives a pointer to the outer function's stack frame,
 * allowing it to access variables from the outer scope. In the following example:
 *  fun A() nothing:
 *      var x : int;
 *      fun B() : nothing
 *      {
 *          x <- x + 1;
 *      }
 *  {
 *      x <- 10;
 *      B();
 *  }
 * 
 * - Function A will have its own stack frame.
 * - Function B will have an additional parameter that is a pointer to A's stack frame struct.
 * - The static link ensures that B can access x from A.
 */

void Header::addStaticLinkToFunctionSignature(std::vector<std::string> *param_names, std::vector<llvm::Type*> *param_types)
{
    // Get the mangled name of the outer function
    std::string outer_func_mangled_name = OuterFunction[mangled_name];
    
    // Get the name of the stack frame structure for the outer function
    std::string outer_func_stack_frame_struct_name = getFunctionStackFrameStructName(outer_func_mangled_name);
    
    // Add a pointer to the outer function's stack frame structure type to the list of types
    param_types->push_back(llvm::StructType::getTypeByName(TheContext, outer_func_stack_frame_struct_name)->getPointerTo());
    
    // Add a name for the static link to the list of names
    param_names->push_back("static_link_" + mangled_name);
}

void Header::addCapturedParameters(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref)
{
    if (fparamlist)
    {
        for(FParam *fparam : fparamlist->getParams())
            fparam->addCapturedParameters(param_names, param_types, ref);
    }
}