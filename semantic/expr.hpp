#ifndef __EXPR_HPP__ 
#define __EXPR_HPP__ 

#include "ast.hpp"


class Expr : public AST
{
public:
    Type getType();
    void type_check(Type t);

protected:
    Type type = nullptr;
};

#endif
