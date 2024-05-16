#include "lvalue.hpp"

void LValue::setScope(int sc) 
{
    scope = sc;
}

int LValue::getScope()
{
    return scope;
}

void LValue::setRef(bool r)
{
    ref = r;
}

bool LValue::getRef()
{
    return ref;
}