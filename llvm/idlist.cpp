#include "idlist.hpp"

IdList::IdList() : idlist() {}
IdList::~IdList()
{
    for (Id *id : idlist)
    {
        delete id;
    }
}

const std::vector<Id *> IdList::getIds()
{
    return idlist;
}

void IdList::append(Id *id)
{
    idlist.push_back(id);
}
void IdList::printOn(std::ostream &out) const
{
    out << "IdList(";
    bool first = true;
    for (auto id = idlist.rbegin(); id != idlist.rend(); ++id)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **id;
    }
    out << ")";
}
void IdList::sem()
{
    for (auto id = idlist.rbegin(); id != idlist.rend(); ++id)
        (*id)->sem();
}

llvm::Value *IdList::compile()
{
    for (auto id = idlist.rbegin(); id != idlist.rend(); ++id)
        (*id)->compile();
    return nullptr;
}