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
    for (auto l = locals.rbegin(); l != locals.rend(); ++l)
    {
        if (!first)
            out << ", ";
        out << **l;
        first = false;
    }
    out << ")";
}

void LocalDefList::append(LocalDef *l)
{
    locals.push_back(l);
}

void LocalDefList::sem() {
    for (auto l = locals.rbegin(); l != locals.rend(); ++l)
    {
        (*l)->sem();
    }
}

llvm::Value* LocalDefList::compile() const
{
    for (auto l = locals.rbegin(); l != locals.rend(); ++l)
    {
        (*l)->compile();
    }
    return nullptr;
}

std::vector<LocalDef*> LocalDefList::getLocals()
{
    return locals;
}