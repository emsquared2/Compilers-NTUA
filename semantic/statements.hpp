#ifndef __STATEMENTS_HPP__
#define __STATEMENTS_HPP__

#pragma once

#include <vector>
#include "ASTabstracts.hpp"
#include "ast.hpp"

/* #####################################################################
   #####                         STATEMENTS                        #####
   ##################################################################### */


/* ---------------------------------------------------------------------
   ------------------------------- BLOCK -------------------------------
   --------------------------------------------------------------------- */

class Block : public Stmt
{
public:
    Block();
    ~Block();
    void append(Stmt *s);
    virtual void printOn(std::ostream &out) const override;
    virtual void run() const override;
    std::vector<Stmt *> getList();
    virtual void sem() override;

private:
    std::vector<Stmt *> stmt_list;
};

/* ---------------------------------------------------------------------
   ------------------------------- ASSIGN ------------------------------
   --------------------------------------------------------------------- */

class Assign : public Stmt
{
public:
    Assign(LValue *l, Expr *e);
    ~Assign();
    virtual void printOn(std::ostream &out) const override;
    virtual void run() const override;
    virtual void sem() override;

private:
    LValue *l_value;
    Expr *expr;
};

/* ---------------------------------------------------------------------
   --------------------------------- IF --------------------------------
   --------------------------------------------------------------------- */

class If : public Stmt
{
public:
    If(Cond *c, Stmt *s1, Stmt *s2);
    ~If();
    virtual void printOn(std::ostream &out) const override;
    virtual void run() const override;
    virtual void sem() override;

private:
    Cond *cond;
    Stmt *stmt1, *stmt2;
};

/* ---------------------------------------------------------------------
   -------------------------------- WHILE ------------------------------
   --------------------------------------------------------------------- */

class While : public Stmt
{
public:
    While(Cond *c, Stmt *s);
    ~While();
    virtual void printOn(std::ostream &out) const override;
    virtual void run() const override;
    virtual void sem() override;

private:
    Cond *cond;
    Stmt *stmt;
};

/* ---------------------------------------------------------------------
   ------------------------------- RETURN ------------------------------
   --------------------------------------------------------------------- */

class Return : public Stmt
{
public:
    Return(Expr *e = nullptr);
    virtual void printOn(std::ostream &out) const override;
    virtual void run() const override;
    int ReturnValue();
    virtual void sem() override;

private:
    Expr *expr;
};

/* ---------------------------------------------------------------------
   ----------------------------- EMPTY STMT ----------------------------
   --------------------------------------------------------------------- */

class EmptyStmt : public Stmt
{
public:
    EmptyStmt();
    virtual void printOn(std::ostream &out) const override;
};


/* ---------------------------------------------------------------------
   ---------------------- FUNCTION CALL STATEMENT ----------------------
   --------------------------------------------------------------------- */

class CallStmt : public Stmt
{
public:
    CallStmt(Id *id, ExprList *e_list);
    ~CallStmt();
    virtual void printOn(std::ostream &out) const override;
    // TODO: Implement how a function is run.
    // virtual int eval() const override
    // {
    // }
    virtual void run() const override;
    virtual void sem() override;

private:
    Id *id;
    ExprList *expr_list;
};

#endif