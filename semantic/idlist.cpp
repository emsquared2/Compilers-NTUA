#include "idlist.hpp"

IdList::IdList() : idlist() {}
IdList::~IdList()
{
    for (Id *id : idlist)
    {
        delete id;
    }
}

const std::vector<Id *> IdList::get_idlist() 
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
    std::cout << "IdList Sem..." << std::endl;
    for (Id *i : idlist)
        i->sem();
}