#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#pragma once

#include "ASTabstracts.hpp"

/* #####################################################################
   #####                         CONDITIONS                        #####
   ##################################################################### */


/* ---------------------------------------------------------------------
   ------------------------------ OpCond -------------------------------
   --------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------
   ----------------------------- CompCond ------------------------------
   --------------------------------------------------------------------- */

class CompCond : public Cond
{
public:
    CompCond(Expr *l, char *s, Expr *r);
    ~CompCond();
    virtual void printOn(std::ostream &out) const override;
    // virtual int eval() const override
    // {
    //     if (op == "=")
    //     {
    //         return left->eval() == right->eval();
    //     }
    //     else if (op == "#")
    //     {
    //         return left->eval() != right->eval();
    //     }
    //     else if (op == "<")
    //     {
    //         return left->eval() < right->eval();
    //     }
    //     else if (op == ">")
    //     {
    //         return left->eval() > right->eval();
    //     }
    //     else if (op == "<=")
    //     {
    //         return left->eval() <= right->eval();
    //     }
    //     else if (op == ">=")
    //     {
    //         return left->eval() >= right->eval();
    //     }

    //     return 0;
    // }
    virtual void sem() override;

private:
    Expr *left;
    char *op;
    Expr *right;
};

#endif