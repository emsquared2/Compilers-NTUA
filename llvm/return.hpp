#ifndef __RETURN_HPP__
#define __RETURN_HPP__

#include "stmt.hpp"
#include "expr.hpp"

class Return : public Stmt
{
public:
    Return(Expr *e = nullptr);
    virtual void printOn(std::ostream &out) const override;
    int ReturnValue();
    virtual void sem() override;
    virtual llvm::Value *compile() const override;

private:
    Expr *expr;
};

#endif