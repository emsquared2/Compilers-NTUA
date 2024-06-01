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