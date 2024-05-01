#ifndef __AST_HPP__
#define __AST_HPP__

#include <map>
#include <string>
#include <vector>
#include <variant>
#include "ASTabstracts.hpp"
#include "types.hpp"
#include "symbol.h"

class FuncDef;
class LocalDefList;
class LocalDef;

/* ---------------------------------------------------------------------
   ----------------------------- ConstStr ------------------------------
   --------------------------------------------------------------------- */

class ConstStr : public LValue
{
public:
    ConstStr(std::string s);
    virtual void printOn(std::ostream &out) const override;
    virtual const char *getStr();
    virtual const char *getName() const override;
    virtual int eval() const override;
    virtual void sem() override;

private:
    std::string str;
};

/* ---------------------------------------------------------------------
   -------------------------------- Id ---------------------------------
   --------------------------------------------------------------------- */

class Id : public LValue
{
public:
    Id(std::string s);
    virtual void printOn(std::ostream &out) const override;
    virtual std::string getId();
    virtual const char *getName() const override;
    virtual int eval() const override;
    virtual void sem() override;

private:
    std::string name;
};

/* ---------------------------------------------------------------------
   ------------------------------ IdList -------------------------------
   --------------------------------------------------------------------- */

class IdList : public AST
{
public:
    IdList();
    ~IdList();
    const std::vector<Id *> get_idlist();
    void append(Id *id);
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    std::vector<Id *> idlist;
};

/* ---------------------------------------------------------------------
   ----------------------------- ExprList ------------------------------
   --------------------------------------------------------------------- */

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

private:
    std::vector<Expr *> expr_list;
};

/* ---------------------------------------------------------------------
   ----------------------------- StmtList ------------------------------
   --------------------------------------------------------------------- */

class StmtList : public AST
{
public:
    StmtList();
    ~StmtList();
    void append(Stmt *s);
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    std::vector<Stmt *> stmt_list;
};

/* ---------------------------------------------------------------------
   ------------------------------ FParam -------------------------------
   --------------------------------------------------------------------- */

class FParam : public AST
{
public:
    FParam(IdList *idl, FParType *t, bool ref);
    ~FParam();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void setSymbolEntry(SymbolEntry *f);

private:
    IdList *idlist;
    FParType *fpartype;
    Type type;
    PassMode pass_mode;
    bool ref;

    SymbolEntry *function;
};

/* ---------------------------------------------------------------------
   ---------------------------- FParamList -----------------------------
   --------------------------------------------------------------------- */

class FParamList : public AST
{
public:
    FParamList();
    ~FParamList();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void append(FParam *param);
    void setSymbolEntry(SymbolEntry *f);

private:
    std::vector<FParam *> params;
    SymbolEntry *function;
};

/* ---------------------------------------------------------------------
   ------------------------------- Decl --------------------------------
   --------------------------------------------------------------------- */

class Decl : public AST
{
public:
    Decl(IdList *idl, FParType *fpt);
    ~Decl();
    virtual void printOn(std::ostream &out) const override;

    virtual void sem() override;

private:
    IdList *idlist;
    FParType *parser_type;
    Type type;
};

// class RetType;

/* ---------------------------------------------------------------------
   ------------------------------ Header -------------------------------
   --------------------------------------------------------------------- */

class Header : public AST
{
public:
    Header(Id *id, RetType *t, FParamList *fpl);
    ~Header();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void set_forward_declaration();

private:
    Id *id;
    RetType *ret_type;
    FParamList *fparamlist;
    Type type; // why does this exist ? (when RetType class is already here)
    bool forward_declaration = false;
};

/* ---------------------------------------------------------------------
   ----------------------------- FuncDecl ------------------------------
   --------------------------------------------------------------------- */

class FuncDecl : public AST
{
public:
    FuncDecl(Header *h);
    ~FuncDecl();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Header *header;
};

/* ---------------------------------------------------------------------
   ----------------------------- ArrayElem -----------------------------
   --------------------------------------------------------------------- */

class ArrayElem : public LValue
{
public:
    ArrayElem(LValue *l, ExprList *e);
    ~ArrayElem();
    virtual void printOn(std::ostream &out) const override;
    virtual std::string getArrayElem();
    virtual int eval() const override;
    virtual void sem() override;

private:
    LValue *left;
    ExprList *exprlist;
};

#endif
