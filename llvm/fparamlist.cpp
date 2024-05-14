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
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        if (!first)
            out << "; ";
        first = false;
        out << **param;
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
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        (*param)->setSymbolEntry(function);
        (*param)->sem();
    }
}

std::vector<llvmType *> FParamList::getLLVM_params()
{
    std::vector<llvmType *> llvm_params;
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        Type t = (*param)->getType();
        llvmType *param_type = getLLVMType(t, TheContext);

        std::vector<Id *> ids = (*param)->getIdList()->getIds();
        for (auto id = ids.rbegin(); id != ids.rend(); ++id)
            llvm_params.push_back(param_type);
    }
    return llvm_params;
}

std::vector<std::string> FParamList::getLLVM_param_names()
{
    std::vector<std::string> llvm_param_names;
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        Type t = (*param)->getType();

        std::vector<Id *> ids = (*param)->getIdList()->getIds();
        for (auto id = ids.rbegin(); id != ids.rend(); ++id) {
            std::string mangled_name = std::string((*id)->getName()) + '_' + std::to_string((*id)->getScope()) + '_';
            llvm_param_names.push_back(mangled_name);
        }
    }
    return llvm_param_names;
}

llvm::Value *FParamList::compile() const
{
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        (*param)->compile();
    }
}