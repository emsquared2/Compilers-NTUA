#ifndef __COND_HPP__ 
#define __COND_HPP__ 

#include "ast.hpp"

class Cond : public AST
{
public:
    Type getType();
    void typeCheck(Type t);

protected:
    Type type = nullptr;
};

#endif