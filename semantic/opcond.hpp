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

    // virtual int eval() const override
    // {
    //     if (op == "not")
    //     {
    //         return !right->eval();
    //     }
    //     else if (op == "and")
    //     {
    //         return left->eval() && right->eval();
    //     }
    //     else if (op == "or")
    //     {
    //         return left->eval() || right->eval();
    //     }
    //     return 0;
    // }

    virtual void sem() override;

private:
    Cond *left;
    char *op;
    Cond *right;
};
#endif