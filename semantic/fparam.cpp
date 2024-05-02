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

void FParam::sem()
{
    std::cout << "FParam Sem..." << std::endl;
    for (Id *id : idlist->get_idlist())
        newParameter(id->getName(), type, pass_mode, function);

    function = nullptr;
}
