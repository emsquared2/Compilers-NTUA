#ifndef __COMPCOND_HPP__
#define __COMPCOND_HPP__

#include "cond.hpp"
#include "expr.hpp"

class CompCond : public Cond
{
public:
    CompCond(Expr *l, const char *s, Expr *r);
    ~CompCond();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Expr *left;
    const char *op;
    Expr *right;
};

#endif