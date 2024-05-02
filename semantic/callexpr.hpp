#ifndef __CALLEXPR_HPP__
#define __CALLEXPR_HPP__

#include "expr.hpp"
#include "id.hpp"
#include "exprlist.hpp"

class CallExpr : public Expr
{
public:
    CallExpr(Id *id, ExprList *e_list);
    ~CallExpr();
    virtual void printOn(std::ostream &out) const override;
    // TODO: Implement how a function is run.
    // virtual int eval() const override
    // {
    // }
    virtual void sem() override;

private:
    Id *id;
    ExprList *expr_list;
};


#endif