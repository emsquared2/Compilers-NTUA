#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

extern std::map<std::string, int> globals;

// enum Type
// {
//     TYPE_INT,
//     TYPE_BOOL
// };

// struct ExprList
// {
//     std::vector<Expr *> expr_list;
// };

class FuncDef;
class LocalDef;
class Local;

class AST
{
public:
    virtual ~AST() {}
    virtual void printOn(std::ostream &out) const = 0;
};

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
}

class Expr : public AST
{
public:
    virtual int eval() const = 0;

protected:
    Type *type;
};

class Cond : public AST
{
public:
    virtual int eval() const = 0;
};

class Stmt : public AST
{
public:
    virtual void run() const = 0;
};

class LValue : public Expr
{
public:
    virtual int eval() const = 0;
    virtual std::string getName() const {};
};

class Const : public Expr
{
public:
    Const(int n) : num(n) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Const(" << num << ")";
    }
    virtual int eval() const override
    {
        return num;
    }

private:
    int num;
};

class ConstChar : public Expr
{
public:
    ConstChar(char v) : var(v) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "ConstChar(" << var << ")";
    }
    virtual int eval() const override
    {
        // return var;
    }

private:
    char var;
};

class ConstStr : public LValue
{
public:
    ConstStr(char *s) : var(s) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "ConstStr(" << var << ")";
    }
    virtual const char *getStr()
    {
        return var;
    }
    virtual std::string getName() const override
    {
        return std::string(var);
    }
    virtual int eval() const override
    {
    }

private:
    const char *var;
};

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
    }

private:
    char *var;
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

private:
    std::vector<Id *> idlist;
};

class ArrayDim : public AST
{
public:
    ArrayDim() : dim() {}
    ~ArrayDim()
    {
        for (Const *num : dim)
        {
            delete num;
        }
    }
    virtual void printOn(std::ostream &out) const override
    {
        for (Const *num : dim)
        {
            out << "[" << *num << "]";
        }
    }
    bool isEmpty()
    {
        return dim.empty();
    }
    void append(Const *num)
    {
        dim.push_back(num);
    }

private:
    std::vector<Const *> dim;
};

class ArrayElem : public LValue
{
public:
    ArrayElem(LValue *l, Expr *e) : left(l), expr(e) {}
    ~ArrayElem()
    {
        delete left;
        delete expr;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "ArrayElem(" << *left << "[" << *expr << "]";
    }
    virtual std::string getArrayElem()
    {
        return left->getName() + "[" + std::to_string(expr->eval()) + "]";
    }
    virtual int eval() const override
    {
    }

private:
    LValue *left;
    Expr *expr;
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

private:
    std::vector<Stmt *> stmt_list;
};

class DataType : public AST
{
public:
    DataType(char *t) : type(t) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "DataType(" << type << ")";
    }

private:
    char *type;
};

class Type : public AST
{
public:
    Type(DataType *t, ArrayDim *d) : type(t), dim(d) {}
    ~Type()
    {
        delete type;
        delete dim;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Type(" << *type << *dim << ")";
    }

private:
    DataType *type;
    ArrayDim *dim;
};

class RetType : public AST
{
public:
    RetType(DataType *t = nullptr) : dtype(t) {}
    ~RetType() { delete dtype; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "RetType(";
        if (dtype == nullptr)
            out << "nothing)";
        else
            out << *dtype << ")";
    }

private:
    DataType *dtype;
};

class FParType : public AST
{
public:
    FParType(DataType *t, ArrayDim *d, bool dl = false) : type(t), dim(d), dimlen(dl) {}
    ~FParType()
    {
        delete type;
        delete dim;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParType(" << *type;
        if (dimlen)
            out << "[ ]";
        out << *dim << ")";
    }

private:
    DataType *type;
    ArrayDim *dim;
    bool dimlen;
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

private:
    Id *id;
    RetType *type;
    ParamList *paramlist;
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

private:
    Cond *cond;
    Stmt *stmt;
};

#endif