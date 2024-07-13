#ifndef __RETURN_HPP__
#define __RETURN_HPP__

#include "stmt.hpp"
#include "expr.hpp"

class Return : public Stmt
{
public:
    Return(Expr *e = nullptr);
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Expr *expr;
};

#endif