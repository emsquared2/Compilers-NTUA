#include "fparamlist.hpp"

FParamList::FParamList() : params() {}
FParamList::~FParamList()
{
    for (FParam *param : params)
    {
        delete param;
    }
}

void FParamList::printOn(std::ostream &out) const
{
    bool first = true;
    out << "FParamList(";
    for (FParam *param : getReversed(params))
    {
        if (!first)
            out << "; ";
        first = false;
        out << *param;
    }
    out << ")";
}

void FParamList::append(FParam *param)
{
    params.push_back(param);
}

void FParamList::setSymbolEntry(SymbolEntry *f)
{
    function = f;
}

void FParamList::sem()
{
    for (FParam *param : getReversed(params))
    {
        param->setSymbolEntry(function);
        param->sem();
    }
}

std::vector<llvmType *> FParamList::getLLVM_params()
{
    std::vector<llvmType *> llvm_params;
    for (FParam *param : getReversed(params))
    {
        Type t = param->getType();

        PassMode pass_mode = param->getPassMode();
        // llvmType *param_type = getLLVMType(t, TheContext);
        llvmType *param_type = (pass_mode == PASS_BY_VALUE) ? getLLVMType(t, TheContext) : llvm::PointerType::get(getLLVMType(t, TheContext), 0);

        for (int i = 0; i < param->getIdList()->getIds().size(); ++i)
            llvm_params.push_back(param_type);
    }
    return llvm_params;
}

std::vector<std::string> FParamList::getLLVM_param_names()
{
    std::vector<std::string> llvm_param_names;
    for (FParam *param : getReversed(params))
    {
        for (Id *id : param->getIdList()->getIds()) {
            std::string mangled_name = std::string(id->getMangledName());
            llvm_param_names.push_back(mangled_name);
        }
    }
    return llvm_param_names;
}

std::vector<FParam *> FParamList::getParams()
{
    return getReversed(params);
}

llvm::Value *FParamList::compile()
{
    for (FParam *param : getReversed(params))
        param->compile();
    return nullptr;
}