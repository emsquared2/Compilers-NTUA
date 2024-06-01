#ifndef __CONST_HPP__
#define __CONST_HPP__

#include "expr.hpp"

class Const : public Expr
{
public:
    Const(int val);
    ~Const();
    virtual void printOn(std::ostream &out) const override;
    int getVal();
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    const int val;
};

#endif