#include "cond.hpp"

Type Cond::getType()
{
    return type;
}

void Cond::type_check(Type t)
{
    sem();

    if (!equalType(type, t))
    {
        SemanticError("Type mismatch");
    }
}