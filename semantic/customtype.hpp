#ifndef __CUSTOMTYPE_HPP__
#define __CUSTOMTYPE_HPP__

#include "ast.hpp"

class CustomType : public AST
{
public:
    virtual void printOn(std::ostream &out) const = 0;
    virtual Type ConvertToType() const = 0;
};

#endif