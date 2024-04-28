#include "ast.hpp"
#include "local.hpp"
#include "localdef.hpp"

LocalDef::LocalDef() : locals() {}
LocalDef::~LocalDef()
{
    for (Local *l : locals)
        delete l;
}

void LocalDef::printOn(std::ostream &out) const
{
    out << "LocalDef(";
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

void LocalDef::sem()
{
    for (Local *l : locals)
        l->sem();
}

void LocalDef::append(Local *l)
{
    locals.push_back(l);
}