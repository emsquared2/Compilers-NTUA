#ifndef __LVALUE_HPP__ 
#define __LVALUE_HPP__ 

#include "expr.hpp"

class LValue : public Expr
{
public:
    virtual const char *getName() const {};
};

#endif
