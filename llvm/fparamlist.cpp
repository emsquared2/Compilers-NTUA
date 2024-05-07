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
