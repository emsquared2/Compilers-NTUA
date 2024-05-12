#include "lvalue.hpp"

void LValue::setScope(int sc) 
{
    scope = sc;
}

int LValue::getScope()
{
    return scope;
}