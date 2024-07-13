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
    return getReversed(idlist);
}

void IdList::append(Id *id)
{
    idlist.push_back(id);
}
void IdList::printOn(std::ostream &out) const
{
    out << "IdList(";
    bool first = true;
    for (Id *id : getReversed(idlist))
    {
        if (!first)
            out << ", ";
        first = false;
        out << *id;
    }
    out << ")";
}
void IdList::sem()
{
    for (Id *i : getReversed(idlist))
        i->sem();
}