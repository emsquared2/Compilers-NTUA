#ifndef __COMPCOND_HPP__
#define __COMPCOND_HPP__

#include "cond.hpp"
#include "expr.hpp"

class CompCond : public Cond
{
public:
    CompCond(Expr *l, char *s, Expr *r);
    ~CompCond();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() const override;

private:
    Expr *left;
    char *op;
    Expr *right;
};

#endif