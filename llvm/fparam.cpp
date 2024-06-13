#include "fparam.hpp"

FParam::FParam(IdList *idl, FParType *t, bool ref = false) : idlist(idl), fpartype(t), ref(ref)
{
    pass_mode = ref ? PASS_BY_REFERENCE : PASS_BY_VALUE;
    type = fpartype->ConvertToType();
}
FParam::~FParam()
{
    delete idlist;
    delete fpartype;
    if (fpartype)
    {
        destroyType(type);
    }
}
void FParam::printOn(std::ostream &out) const
{
    out << "FParam(";
    if (ref)
        out << "ref ";
    out << *idlist << " : ";
    out << *fpartype;
    // if (fpartype) printType(fpartype);
    out << ")";
}

void FParam::setSymbolEntry(SymbolEntry *f)
{
    function = f;
}

IdList *FParam::getIdList()
{
    return idlist;
}

void FParam::sem()
{
    /* Arrays passed as function parameters can only be passed by reference */
    bool isArrayType = !equalType(type, typeInteger) && !equalType(type, typeChar);
    if (isArrayType && pass_mode != PASS_BY_REFERENCE)
    {
        SemanticError("Arrays can only be passed by reference.");
    }

    SymbolEntry *param;
    for (Id *id : idlist->getIds()) {
        param = newParameter(id->getName(), type, pass_mode, function);
        // id->setScope(param->scopeId);        
    }

    scope = param->scopeId;

    function = nullptr;
}

Type FParam::getType()
{
    return type;
}

PassMode FParam::getPassMode()
{
    return pass_mode;
}

llvm::Value *FParam::compile()
{
    return LogErrorV("FParam->compile() should never be called!");
}

llvm::Value* FParam::compile(std::vector<std::string> * signature_mangled_names, std::vector<llvmType*> * signature_types)
{
    llvmType* t;
    switch(pass_mode)
    {
    case PASS_BY_VALUE:
        t = getLLVMType(type, TheContext);
        break;
    case PASS_BY_REFERENCE:
        t = llvm::PointerType::get(getLLVMType(type, TheContext), 0);
        break;
    }
    for (Id *id : idlist->getIds())
    {
        signature_types->push_back(t);
        signature_mangled_names->push_back(getMangledName(id->getName(), scope));
    }

return nullptr;
}

/*
 * This function adds captured variables (parameters) to the function's parameter lists if they are used in an inner function.
 * It ensures that the innfer function's parameter list includes pointers to these captured variables so that they can be accessed
 * within the inner function. For example:
 * fun outer(x : int) : nothing
 *      fun inner() : nothing
 *      {
 *          x <- x + 1;
 *      }
 * {
 *      x <- 10;
 *      inner();
 * }
 * 
 * - Variable x is a parameter of `outer` and is captured by `inner`, so var x needs to be included in `inner`'s parameter list.
 */
void FParam::addCapturedParameters(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref)
{
    llvmType *t = llvm::PointerType::get(getLLVMType(type, TheContext), 0);

    if (idlist) {
        for (Id *id : idlist->getIds()) {
            std::string mangled_name = getMangledName(id->getName(), scope);
            if(CapturedVariables.find(mangled_name) != CapturedVariables.end()) {
                param_names->push_back(mangled_name);
                param_types->push_back(t);
                CapturedVariableOffset[mangled_name] = param_names->size() - 1;
                ref->push_back(pass_mode == PASS_BY_REFERENCE);
            }
        }
    }
}