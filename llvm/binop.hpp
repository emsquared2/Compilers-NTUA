#ifndef __BINOP_HPP__
#define __BINOP_HPP__

#include "expr.hpp"

class BinOp : public Expr
{
public:
    BinOp(Expr *l, char *s, Expr *r);
    ~BinOp();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    Expr *left;
    const char *op;
    Expr *right;
};

#endif