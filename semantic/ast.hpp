#ifndef __AST_HPP__
#define __AST_HPP__

#include <map>
#include <string>
#include <vector>
#include <variant>
#include "ASTabstracts.hpp"
#include "types.hpp"
#include "symbol.h"

// extern std::map<std::string, int> globals;

class FuncDef;
class LocalDef;
class Local;

class Id : public LValue
{
public:
    Id(char *s) : var(s) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Id(" << var << ")";
    }
    virtual std::string getId()
    {
        return var;
    }
    virtual std::string getName() const override
    {
        return var;
    }
    virtual int eval() const override
    {
        // return rt_stack[offset]
    }
    virtual void sem() override
    {
        SymbolEntry *e = st.lookup(var);
        if (e == nullptr)
        {
            std::cerr << "Undeclared variable " << var << std::endl;
            throw std::runtime_error("Semantic error: Undeclared variable");
        }
        else
        {
            type = e->getType();
            offset = e->getOffset();
        }
    }

private:
    char *var;
    int offset;
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
        // for (Stmt *s : stmt_list)
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

class Block : public Stmt
{
public:
    Block() : stmt_list() {}
    ~Block()
    {
        for (Stmt *s : stmt_list)
            delete s;
    }
    void append(Stmt *s) { stmt_list.push_back(s); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Block(";
        bool first = true;
        // for (Stmt *s : stmt_list)
        for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
        {
            if (!first)
                out << ", ";
            first = false;
            out << **s;
        }
        out << ")";
    }
    virtual void run() const override
    {
        // for (Stmt *s : stmt_list)
        //     s->run();
    }
    std::vector<Stmt *> getList()
    {
        return stmt_list;
    }
    virtual void sem() override
    {
        for (Stmt *s : stmt_list)
            s->sem();
    }

private:
    std::vector<Stmt *> stmt_list;
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

class FParam : public AST
{
public:
    FParam(IdList *idl, FParType *t, bool ref = false) : idlist(idl), fpartype(t), ref(ref) {}
    ~FParam() { delete idlist; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParam(";
        if (ref)
            out << "ref ";
        out << *idlist << " : " << *fpartype << ")";
    }

private:
    IdList *idlist;
    FParType *fpartype;
    bool ref;
};

class ParamList : public AST
{
public:
    ParamList() : params() {}
    ~ParamList()
    {
        for (FParam *param : params)
        {
            delete param;
        }
    }
    virtual void printOn(std::ostream &out) const override
    {
        bool first = true;
        out << "ParamList(";
        // for (FParam *param : params)
        for (auto param = params.rbegin(); param != params.rend(); ++param)
        {
            if (!first)
                out << "; ";
            first = false;
            out << **param;
        }
        out << ")";
    }

    /*

    Check class Header for information about this sem() method.

    virtual void sem() override {}
    */

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
    Decl(IdList *idl, Type *t) : idlist(idl), type(t) {}
    ~Decl()
    {
        delete idlist;
        delete type;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Decl(var " << *idlist << " : " << *type << ")";
    }

    virtual void sem() override
    {
        for (Id *id : idlist->get_idlist())
        {
            lookupEntry(id->getName(), LOOKUP_CURRENT_SCOPE, true);            
            newVariable(id->getName(), type);

        }
    }

private:
    IdList *idlist;
    Type *type;
};

class Header : public AST
{
public:
    Header(Id *id, RetType *t, ParamList *pl = nullptr) : id(id), type(t), paramlist(pl) {}
    ~Header()
    {
        delete id;
        delete paramlist;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Header(fun " << *id << "(";
        if (paramlist != nullptr)
            out << *paramlist;
        out << ") : " << *type;
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
        SymbolEntry *function = newFunction(id->getName());
        // add case when parser identifies forward declaration
        if (forward_declaration)
            forwardFunction(function);

        /*
        Here we have to add all the parameters to the symbol entry.
        The SymbolEntry of the function (look *function above) is also needed when creating a new function parameter. 
        We have 2 options:
        Option 1: Pass the function SymbolEntry to the ParamList and handle the sem() function inside the ParamList class
        Option 2: Add a method to the ParamList class that returns the private vector of Parameters. Then handle each Param in this sem() function.

        Option 1 might be easier to implement but having a field of SymbolEntry on a Class might seem a little be counterintuitive.

        Option 2 on the other hand might be a cleaner option as the SymbolEntry is abstracted from the classes.
        */
        paramlist->sem();
        /*
        for (FParam *fp : paramlist):
            for (Id *id : fp->getIdlist()):
                SymbolEntry *fun_param = newParameter(id->getName(), ..., ..., function)

                // newParameter (const char * name, Type type, PassMode mode, SymbolEntry * f)

        */

        endFunctionHeader(function, type);
    }

    void set_forward_declaration()
    {
        forward_declaration = true;
    }

private:
    Id *id;
    RetType *type;
    ParamList *paramlist;
    bool forward_declaration = false;
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
        // FuncDecl is for forward declaring function.
        // Header class is for both forward and non forward functions.
        // Set forward_declaration to true to specify a header that represents a forward declared function.
        header->set_forward_declaration();
        header->sem();
    }

private:
    Header *header;
};

class Assign : public Stmt
{
public:
    Assign(LValue *l, Expr *e) : l_value(l), expr(e) {}
    ~Assign()
    {
        delete l_value;
        delete expr;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Assign(" << *l_value << " <- " << *expr << ")";
    }
    virtual void run() const override
    {
        // globals[l_value->getName()] = expr->eval();
    }
    virtual void sem() override
    {
        lookupEntry(l_value->getName(), LOOKUP_ALL_SCOPES, true);
        // check expr type with l_value type
    }

private:
    LValue *l_value;
    Expr *expr;
};

class CallExpr : public Expr
{
public:
    CallExpr(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
    ~CallExpr()
    {
        delete id;
        delete expr_list;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "CallExpr(" << *id << "(";
        if (expr_list != nullptr)
            out << *expr_list;
        out << "))";
    }
    // TODO: Implement how a function is run.
    virtual int eval() const override
    {
    }
    virtual void sem() override
    {
        // Check if the function exists
        SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);


    }

private:
    Id *id;
    ExprList *expr_list;
};

class CallStmt : public Stmt
{
public:
    CallStmt(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
    ~CallStmt()
    {
        delete id;
        delete expr_list;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "CallStmt(" << *id << "(";
        if (expr_list != nullptr)
            out << *expr_list;
        out << "))";
    }
    // TODO: Implement how a function is run.
    // virtual int eval() const override
    // {
    // }
    virtual void run() const override
    {
    }
    virtual void sem() override
    {
        lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);
    }

private:
    Id *id;
    ExprList *expr_list;
};

class UnOp : public Expr
{
public:
    UnOp(char *s, Expr *e) : op(s), right(e) {}
    ~UnOp()
    {
        delete right;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << op << "(" << *right << ")";
    }
    virtual int eval() const override
    {
        if (op == "+")
        {
            return right->eval();
        }
        else if (op == "-")
        {
            return -right->eval();
        }
    }
    virtual void sem() override
    {
        right->type_check(new Integer());
    }

private:
    char *op;
    Expr *right;
};

class BinOp : public Expr, public Cond
{
public:
    BinOp(Expr *l, char *s, Expr *r) : left(l), op(s), right(r) {}
    ~BinOp()
    {
        delete left;
        delete right;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << op << "(" << *left << ", " << *right << ")";
    }
    virtual int eval() const override
    {
        if (op == "+")
        {
            return left->eval() + right->eval();
        }
        else if (op == "-")
        {
            return left->eval() - right->eval();
        }
        else if (op == "*")
        {
            return left->eval() * right->eval();
        }
        else if (op == "div")
        {
            return left->eval() / right->eval();
        }
        else if (op == "mod")
        {
            return left->eval() % right->eval();
        }
        else if (op == "=")
        {
            return left->eval() == right->eval();
        }
        else if (op == "#")
        {
            return left->eval() != right->eval();
        }
        else if (op == "<")
        {
            return left->eval() < right->eval();
        }
        else if (op == ">")
        {
            return left->eval() > right->eval();
        }
        else if (op == "<=")
        {
            return left->eval() <= right->eval();
        }
        else if (op == ">=")
        {
            return left->eval() >= right->eval();
        }

        return 0;
    }
    virtual void sem() override
    {
        left->type_check(new Integer());
        right->type_check(new Integer());
    }

private:
    Expr *left;
    char *op;
    Expr *right;
};

class OpCond : public Cond
{
public:
    OpCond(Cond *l, char *s, Cond *r) : left(l), op(s), right(r) {}
    OpCond(char *s, Cond *r) : left(), op(s), right(r) {}
    ~OpCond()
    {
        delete left;
        delete right;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << op << "(";
        if (left != nullptr)
            out << *left << ", ";
        out << *right << ")";
    }
    virtual int eval() const override
    {
        if (op == "not")
        {
            return !right->eval();
        }
        else if (op == "and")
        {
            return left->eval() && right->eval();
        }
        else if (op == "or")
        {
            return left->eval() || right->eval();
        }
        return 0;
    }

    virtual void sem() override
    {
        if (left != nullptr)
            left->type_check(new Boolean());
        right->type_check(new Boolean());
    }

private:
    Cond *left;
    char *op;
    Cond *right;
};

class Return : public Stmt
{
public:
    Return(Expr *e = nullptr) : expr(e) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Return(";
        if (expr != nullptr)
            out << *expr;
        out << ")";
    }
    virtual void run() const override
    {
    }
    int ReturnValue()
    {
        return (expr != nullptr) ? expr->eval() : 0;
    }

    virtual void sem() override
    {
        // Should lookup for the function's type and type_check...
        // function Scope
        // Type expected_type = ???
        // expr->sem();
    }

private:
    Expr *expr;
};

class If : public Stmt
{
public:
    If(Cond *c, Stmt *s1, Stmt *s2 = nullptr) : cond(c), stmt1(s1), stmt2(s2) {}
    ~If()
    {
        delete cond;
        delete stmt1;
        delete stmt2;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "If(" << *cond << ", " << *stmt1;
        if (stmt2 != nullptr)
            out << " Else " << *stmt2;
        out << ")";
    }
    virtual void run() const override
    {
        if (cond->eval())
            stmt1->run();
        else if (stmt2 != nullptr)
            stmt2->run();
    }

    virtual void sem() override
    {
        cond->type_check(new Boolean());
        stmt1->sem();
        if (stmt2 != nullptr)
            stmt2->sem();
    }

private:
    Cond *cond;
    Stmt *stmt1, *stmt2;
};

class While : public Stmt
{
public:
    While(Cond *c, Stmt *s) : cond(c), stmt(s) {}
    ~While()
    {
        delete cond;
        delete stmt;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "While(" << *cond << ", " << *stmt << ")";
    }
    virtual void run() const override
    {
        while (cond->eval())
            stmt->run();
    }

    virtual void sem() override
    {
        cond->type_check(new Boolean());
        stmt->sem();
    }

private:
    Cond *cond;
    Stmt *stmt;
};

#endif