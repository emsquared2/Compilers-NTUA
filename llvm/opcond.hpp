#ifndef __OPCOND_HPP__
#define __OPCOND_HPP__

#include "cond.hpp"

class OpCond : public Cond
{
public:
    OpCond(Cond *l, char *s, Cond *r);
    OpCond(char *s, Cond *r);
    ~OpCond();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value *compile() override;

private:
    Cond *left;
    char *op;
    Cond *right;
};
#endif