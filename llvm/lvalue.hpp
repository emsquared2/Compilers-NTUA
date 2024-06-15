#ifndef __LVALUE_HPP__ 
#define __LVALUE_HPP__ 

#include "expr.hpp"

class LValue : public Expr
{
public:
    virtual const char *getName() const;

    void setScope(int sc);
    int getScope();
    void setRef(bool r);
    bool getRef();

protected:
    int scope = -1;
    bool ref = false;
    std::string name;

    // Indicates whether to insert a zero index at the beginning of the offsets for array dereferencing.
    // This is needed when the base address points to an array and we need to navigate to its elements.
    bool dereference;
};

#endif
