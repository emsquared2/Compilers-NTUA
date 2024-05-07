
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

// class Block;
// class CallStmt;

class ConstStr : public LValue
{
public:
    ConstStr(std::string s) : str(s) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "ConstStr(" << str << ")";
    }
    virtual const char *getStr()
    {
        return str.c_str();
    }
    virtual const char * getName() const override
    {
        return str.c_str();
    }
    virtual int eval() const override
    {
    }
    virtual void sem() override {
        RepInteger len = str.length() + 1;
        type = typeArray(len, typeChar);
    }

private:
    std::string str;
};

class Id : public LValue
{
public:
    Id(std::string s) : name(s) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Id(" << name << ")";
    }
    virtual std::string getId()
    {
        return name.c_str();
    }
    virtual const char * getName() const override
    {
        return name.c_str();
    }
    virtual int eval() const override
    {
        // return rt_stack[offset]
    }
    virtual void sem() override
    {
        // SymbolEntry *e = st.lookup(name);
        // if (e == nullptr)
        // {
        //     std::cerr << "Undeclared variable " << name << std::endl;
        //     throw std::runtime_error("Semantic error: Undeclared variable");
        // }
        // else
        // {
        //     type = e->getType();
        //     offset = e->getOffset();
        // }
    }

private:
    std::string name;
};

class IdList : public AST
{
public:
    IdList() : idlist() {}
    ~IdList()
    {
        for (Id *id : idlist)
        {
            delete id;
        }
    }

    const std::vector<Id *> get_idlist() { return idlist; }

    void append(Id *id)
    {
        idlist.push_back(id);
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "IdList(";
        bool first = true;
        // for (Id *id : idlist)
        for (auto id = idlist.rbegin(); id != idlist.rend(); ++id)
        {
            if (!first)
                out << ", ";
            first = false;
            out << **id;
        }
        out << ")";
    }
    virtual void sem() override
    {
        for (Id *i : idlist)
            i->sem();
    }

private:
    std::vector<Id *> idlist;
};

class ExprList : public AST
{
public:
    ExprList() : expr_list() {}
    ~ExprList()
    {
        for (Expr *expr : expr_list)
        {
            delete expr;
        }
    }
    void append(Expr *e) { expr_list.push_back(e); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "ExprList(";
        bool first = true;
        // for (Expr *e : expr_list)
        for (auto e = expr_list.rbegin(); e != expr_list.rend(); ++e)
        {
            if (!first)
                out << ", ";
            first = false;
            out << **e;
        }
        out << ")";
    }
    std::vector<Expr *> getExprList()
    {
        return expr_list;
    }
    bool isEmpty()
    {
        return expr_list.empty();
    }
    virtual void sem() override
    {
        for (Expr *e : expr_list)
            e->sem();
    }

private:
    std::vector<Expr *> expr_list;
};

class StmtList : public AST
{
public:
    StmtList() : stmt_list() {}
    ~StmtList()
    {
        for (Stmt *stmt : stmt_list)
        {
            delete stmt;
        }
    }
    void append(Stmt *s) { stmt_list.push_back(s); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "StmtList(";
        bool first = true;
        for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
        {
            if (!first)
                out << ", ";
            first = false;
            out << **s;
        }
        out << ")";
    }
    virtual void sem() override
    {
        for (Stmt *s : stmt_list)
            s->sem();
    }

private:
    std::vector<Stmt *> stmt_list;
};

class FParam : public AST
{
public:
    FParam(IdList *idl, FParType *t, bool ref = false) : idlist(idl), fpartype(t), ref(ref) {
        pass_mode = ref ? PASS_BY_REFERENCE : PASS_BY_VALUE;
        type = fpartype->ConvertToType(fpartype);
    }
    ~FParam() { 
        delete idlist;
        delete fpartype;
        if (fpartype) { destroyType(type); }
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParam(";
        if (ref)
            out << "ref ";
        out << *idlist << " : ";
        out << *fpartype;
        // if (fpartype) printType(fpartype);
        out << ")";
    }

private:
    IdList *idlist;
    FParType *fpartype;
    Type type;
    PassMode pass_mode;
    bool ref;
};

class FParamList : public AST
{
public:
    FParamList() : params() {}
    ~FParamList()
    {
        for (FParam *param : params)
        {
            delete param;
        }
    }
    virtual void printOn(std::ostream &out) const override
    {
        bool first = true;
        out << "FParamList(";
        for (auto param = params.rbegin(); param != params.rend(); ++param)
        {
            if (!first)
                out << "; ";
            first = false;
            out << **param;
        }
        out << ")";
    }
    void append(FParam *param)
    {
        params.push_back(param);
    }

private:
    std::vector<FParam *> params;
};

class Decl : public AST
{
public:
    Decl(IdList *idl, FParType *fpt) : idlist(idl), parser_type(fpt) { type = parser_type->ConvertToType(parser_type); }
    ~Decl()
    {
        delete idlist;
        delete parser_type;
        if (type) { destroyType(type); }
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Decl(var " << *idlist << " : " << *parser_type;
        // if(type) { printType(type); }
        out << ")";
    }

    virtual void sem() override
    {
        // for (Id *id : idlist->get_idlist())
        // {
        //     st.lookup(id->getId());
        //     st.insert(id->getId(), type);
        // }
    }

private:
    IdList *idlist;
    FParType *parser_type;
    Type type;
};

// class RetType;

class Header : public AST
{
public:
    Header(Id *id, RetType *t, FParamList *fpl = nullptr) : id(id), ret_type(t), fparamlist(fpl) { type = ret_type->ConvertToType(ret_type); }
    ~Header()
    {
        delete id;
        if (fparamlist != nullptr) { delete fparamlist; }
        delete ret_type;
        if (type) { destroyType(type); }
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Header(fun " << *id << "(";
        if (fparamlist != nullptr)
            out << *fparamlist;
        out << ") : " ;
        out << *ret_type;
        // if(type) printType(type);
        out << ")";
    }

    virtual void sem() override
    {
        /* Check if id exists in symbol table. If it exists, throw error.
         * If not:
         *  1) add it to the symbol table
         *  2) open a new scope
         *  3) store the return type
         *  4) paramlit->sem()
         *  5) close the scope?
         */
    }

private:
    Id *id;
    RetType *ret_type;
    FParamList *fparamlist;
    Type type;
};

class FuncDecl : public AST
{
public:
    FuncDecl(Header *h) : header(h) {}
    ~FuncDecl() { delete header; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FuncDecl(" << *header << ")";
    }

    virtual void sem() override
    {
        // Add funcdecl to symbol entry
        header->sem();
    }

private:
    Header *header;
};

class ArrayElem : public LValue
{
public:
    ArrayElem(LValue *l, ExprList *e) : left(l), exprlist(e) {}
    ~ArrayElem()
    {
        delete left;
        delete exprlist;
    }
    virtual void printOn(std::ostream &out) const override
    {
        std::vector<Expr *> exprs = exprlist->getExprList();
        out << "ArrayElem(" << *left;
        for (auto e = exprs.rbegin(); e != exprs.rend(); ++e)
        {
            out << "[" << **e << "]";
        }
        out << ")";
    }
    virtual std::string getArrayElem()
    {
        // return left->getName() + "[" + std::to_string(expr->eval()) + "]";
    }
    virtual int eval() const override
    {
    }
    virtual void sem() override
    {
    }

private:
    LValue *left;
    ExprList *exprlist;
};

#endif