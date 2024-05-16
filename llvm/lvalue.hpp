#ifndef __LVALUE_HPP__ 
#define __LVALUE_HPP__ 

#include "expr.hpp"

class LValue : public Expr
{
public:
    virtual const char *getName() const {};
    virtual llvm::Value * compile_ptr() const {};

    void setScope(int sc);
    int getScope();
    void setRef(bool r);
    bool getRef();

protected:
    int scope = -1;
    bool ref;
};

#endif
