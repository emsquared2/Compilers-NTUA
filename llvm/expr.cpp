#include "expr.hpp"

Type Expr::getType()
{
    return type;
}

void Expr::typeCheck(Type t)
{
    sem();

    if (!equalType(type, t))
        SemanticError("Type mismatch");
}