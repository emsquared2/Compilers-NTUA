#ifndef __EXPRESSIONS_HPP__
#define __EXPRESSIONS_HPP__

#pragma once

#include <vector>
#include "ASTabstracts.hpp"
#include "ast.hpp"

/* #####################################################################
   #####                         EXPRESSIONS                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- UnOp --------------------------------
   --------------------------------------------------------------------- */

class UnOp : public Expr
{
public:
    UnOp(char *s, Expr *e);
    ~UnOp();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    // virtual int eval() const override
    // {
    //     if (op == "+")
    //     {
    //         return right->eval();
    //     }
    //     else if (op == "-")
    //     {
    //         return -right->eval();
    //     }
    // }

private:
    const char *op;
    Expr *right;
};

/* ---------------------------------------------------------------------
   ------------------------------- BinOp -------------------------------
   --------------------------------------------------------------------- */

class BinOp : public Expr
{
public:
    BinOp(Expr *l, char *s, Expr *r);
    ~BinOp();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

    // virtual int eval() const override
    // {
    //     if (op == "+")
    //     {
    //         return left->eval() + right->eval();
    //     }
    //     else if (op == "-")
    //     {
    //         return left->eval() - right->eval();
    //     }
    //     else if (op == "*")
    //     {
    //         return left->eval() * right->eval();
    //     }
    //     else if (op == "div")
    //     {
    //         return left->eval() / right->eval();
    //     }
    //     else if (op == "mod")
    //     {
    //         return left->eval() % right->eval();
    //     }
    //     else if (op == "=")
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

private:
    Expr *left;
    const char *op;
    Expr *right;
};

/* #####################################################################
   #####                         CONST TYPES                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- Const -------------------------------
   --------------------------------------------------------------------- */

class Const : public Expr
{
public:
    Const(int val);
    ~Const();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    // virtual int eval() const override
    // {
    //     return val;
    // }
    int getVal()
    {
        return val;
    }

private:
    const int val;
};

/* ---------------------------------------------------------------------
   ----------------------------- ConstChar -----------------------------
   --------------------------------------------------------------------- */

class ConstChar : public Expr
{
public:
    ConstChar(char v);
    ~ConstChar();
    virtual void printOn(std::ostream &out) const override;
    // virtual int eval() const override
    // {
    //     // return var;
    // }
    virtual void sem() override;

private:
    char var;
};

/* ---------------------------------------------------------------------
   ------------------------------ Nothing ------------------------------
   --------------------------------------------------------------------- */

class Nothing : public Expr
{
public:
    Nothing();
    ~Nothing();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
};

/* ---------------------------------------------------------------------
   ---------------------- FUNCTION CALL EXPRESSION ---------------------
   --------------------------------------------------------------------- */

class Id;
class ExprList;

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
    ExprList *expr_list = nullptr;
};

#endif