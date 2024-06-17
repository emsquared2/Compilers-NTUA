#include "fparamlist.hpp"

FParamList::FParamList() : params() {}
FParamList::~FParamList()
{
    for (FParam *param : params)
        delete param;
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

llvm::Value* FParamList::compile(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types)
{
    for (FParam *param : getReversed(params)) {
        param->compile(param_names, param_types);
    }

    return nullptr;
}