#include "lvalue.hpp"

const char * LValue::getName() const
{
    const char* name = "LValue dummy name";
    return name;
}

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

bool LValue::getDereference()
{
    return dereference;
}
