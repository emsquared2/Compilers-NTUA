#ifndef __EXPRLIST_HPP__
#define __EXPRLIST_HPP__

#include "expr.hpp"
#include <vector>

class ExprList : public AST
{
public:
    ExprList();
    ~ExprList();
    void append(Expr *e);
    virtual void printOn(std::ostream &out) const override;
    std::vector<Expr *> getExprList();
    bool isEmpty();
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    std::vector<Expr *> expr_list;
};

#endif