#ifndef __LOCALDEF_HPP__
#define __LOCALDEF_HPP__

#include "ast.hpp"

class LocalDef : public AST
{
public:
    virtual void setOuterFunction(std::string outer_func_name) = 0;
};

#endif