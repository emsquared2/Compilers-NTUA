#include "expr.hpp"

Type Expr::getType()
{
    return type;
}

void Expr::type_check(Type t)
{
    sem();

    if (!equalType(type, t))
    {
        SemanticError("Type mismatch");
    }
}