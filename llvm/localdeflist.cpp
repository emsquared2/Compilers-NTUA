#include "localdeflist.hpp"

LocalDefList::LocalDefList() : locals() {}
LocalDefList::~LocalDefList()
{
    for (LocalDef *l : locals)
    {
        delete l;
    }
}
void LocalDefList::printOn(std::ostream &out) const
{
    out << "LocalDefList(";
    bool first = true;
    for (LocalDef *l : getReversed(locals))
    {
        if (!first)
            out << ", ";
        out << *l;
        first = false;
    }
    out << ")";
}

void LocalDefList::append(LocalDef *l)
{
    locals.push_back(l);
}

void LocalDefList::sem() {
    for (LocalDef *l : getReversed(locals))
        l->sem();
}

std::vector<LocalDef*> LocalDefList::getLocals()
{
    return getReversed(locals);
}

llvm::Value* LocalDefList::compile()
{
    for (LocalDef *l : getReversed(locals))
    l->compile();
    return nullptr;
}
