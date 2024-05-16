#ifndef __ARRAYELEM_HPP__
#define __ARRAYELEM_HPP__

#include "lvalue.hpp"
#include "exprlist.hpp"

class ArrayElem : public LValue
{
public:
    ArrayElem(LValue *l, ExprList *e);
    ~ArrayElem();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value *compile() override;
    virtual llvm::Value *compile_ptr() override;

private:
    LValue *left;
    ExprList *exprlist;
};

#endif