#include "header.hpp"

Header::Header(Id *id, RetType *t, FParamList *fpl = nullptr) : id(id), ret_type(t), fparamlist(fpl)
{
    type = ret_type->ConvertToType();
}

Header::~Header()
{
    delete id;
    if (fparamlist != nullptr)
        delete fparamlist;
    delete ret_type;
    if (type)
        destroyType(type);
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

void Header::setForwardDeclaration()
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
    // Create a new symbol entry for the function
    SymbolEntry *function = newFunction(id->getName());

    mangled_name = getMangledName(id->getName(), function->scopeId);
    FunctionDepth[mangled_name] = function->nestingLevel;

    // Handle forward declaration of the function
    if (forward_declaration)
        forwardFunction(function);

    // Open a new scope for the function's body
    openScope();
    // Indicate that the function has not a return statement yet
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
    // If the function is not a top-level function, then add a static link to the function signature.
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
    return llvm_param_types;
}

std::vector<std::string> Header::getLLVM_param_names()
{
    return llvm_param_names;
}

/*
 * This function adds a static link to the function signature, allowing inner functions to access
 * variables from the outer function's scope.
 * Example:
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
 * Here:
 * - Function A will have its own stack frame.
 * - Function B will have an additional parameter that is a pointer to A's stack frame.
 * - The static link ensures that B can access the variable x defined in A.
 */
void Header::addStaticLinkToFunctionSignature(std::vector<std::string> *param_names, std::vector<llvm::Type*> *param_types)
{
    // Get the mangled name of the outer function
    std::string outer_func_mangled_name = OuterFunction[mangled_name];
    
    // Get the name of the stack frame structure for the outer function
    std::string outer_func_stack_frame_struct_name = getFunctionStackFrameStructName(outer_func_mangled_name);
    
    // Add a pointer to the outer function's stack frame structure to the parameter types
    param_types->push_back(llvm::StructType::getTypeByName(TheContext, outer_func_stack_frame_struct_name)->getPointerTo());
    
    // Add a name for the static link to the parameter names
    param_names->push_back("static_link_" + mangled_name);
}

// It adds captured parameters to the function signature
void Header::addCapturedParametersToSignature(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref)
{
    if (fparamlist)
    {
        for(FParam *fparam : fparamlist->getParams())
            fparam->addCapturedParameters(param_names, param_types, ref);
    }
}