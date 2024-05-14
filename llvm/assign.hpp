#ifndef __ASSIGN_HPP__
#define __ASSIGN_HPP__

#include "stmt.hpp"
#include "lvalue.hpp"

class Assign : public Stmt
{
public:
    Assign(LValue *l, Expr *e);
    ~Assign();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    LValue *l_value;
    Expr *expr;
};

#endif