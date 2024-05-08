#ifndef __UNOP_HPP__
#define __UNOP_HPP__

#include "expr.hpp"

class UnOp : public Expr
{
public:
    UnOp(char *s, Expr *e);
    ~UnOp();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() const override;

private:
    const char *op;
    Expr *right;
};

#endif