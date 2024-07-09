#include "cond.hpp"

Type Cond::getType()
{
    return type;
}

void Cond::typeCheck(Type t)
{
    sem();

    if (!equalType(type, t))
        SemanticError("Type mismatch");
}