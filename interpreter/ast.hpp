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
    virtual std::string getName() const = 0;
};

class ConstChar : public Expr
{
public:
    ConstChar(char v) : var(v) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "ConstChar(" << var << ")";
    }
    // virtual char eval() const override
    // {
    //     return var;
    // }

private:
    char var;
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

class Id : public LValue
{
public:
    Id(std::string s) : var(s) {}
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

private:
    std::string var;
};

class FuncDef : public AST
{
public:
    FuncDef(Header *h, LocalDef *ld, Block *b) : header(h), localdef(ld), block(b) {}
    ~FuncDef()
    {
        delete header;
        delete localdef;
        delete block;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FuncDef(";
        out << *header << *localdef << *block;
        out << ")";
    }

private:
    Header *header;
    LocalDef *localdef;
    Block *block;
};

class LocalDef : public AST
{
public:
    LocalDef() : locals() {}
    ~LocalDef()
    {
        for (std::variant<FuncDef *, FuncDecl *, Decl *> &local : locals)
        {
            std::visit([](auto &&arg)
                       { delete arg; },
                       local);
        }
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "LocalDef(";
        for (const auto &local : locals)
        {
            std::visit([&out](auto &&arg)
                       { out << *arg; },
                       local);
        }
        out << ")";
    }
    template <typename T>
    void append(T *object)
    {
        locals.push_back(object);
    }

private:
    std::vector<std::variant<FuncDef *, FuncDecl *, Decl *>> locals;
};

class Type : public AST 
{
    public:
    Type(DataType *t, ArrayDim *d) : type(t), dim(d) {}
    ~Type() {
        delete type;
        delete dim;
    }
    virtual void printOn(std::ostream &out) const override {
        out << "Type(" << *type << *dim <<")";
    }
    private:
    DataType *type;
    ArrayDim *dim;
};

class FParam : public AST
{
public:
    FParam(IdList *idl, Type *t, bool ref = false) : idlist(idl), type(t), ref(ref) {}
    ~FParam() { delete idlist; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParam(";
        if (ref)
            out << "ref ";
        out << *idlist << " : " << *type;
    }

private:
    IdList *idlist;
    Type *type;
    bool ref;
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
        for (Id *id : idlist)
        {
            if (!first)
                out << ", ";
            first = false;
            out << id;
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
            out << "[" << num << "]";
        }
    }
    bool isEmpty() {
        return dim.empty();
    }
    void append(Const *num)
    {
        dim.push_back(num);
    }

private:
    std::vector<Const *> dim;
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
        for (Expr *e : expr_list)
        {
            if (!first)
                out << ",";
            first = false;
            out << *e;
        }
        out << ")";
    }

private:
    std::vector<Expr *> expr_list;
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
        bool last = true;
        for (FParam *param : params)
        {
            out << param;
            if (param != params.back())
                out << " ; ";
        }
    }
    void append(FParam *param)
    {
        params.push_back(param);
    }

private:
    std::vector<FParam *> params;
};

class Header : public AST
{
public:
    Header(Id *id, Type *t, ParamList *pl = nullptr) : id(id), type(t), paramlist(pl) {}
    ~Header()
    {
        delete id;
        delete paramlist;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Header(fun " << id << "(";
        if (paramlist != nullptr)
            out << paramlist;
        out << ") :" << *type;
    }

private:
    Id *id;
    Type *type;
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

class Decl : public AST
{
public:
    Decl(IdList *idl, Type *t) : idlist(idl), type(t) {}
    ~Decl()
    {
        delete idlist;
    }
    virtual void printOn(std::ostream &out)
    {
        out << "Decl(var " << idlist << " : " << type << ")";
    }

private:
    IdList *idlist;
    Type *type;
};

class ConstStr : public LValue
{
public:
    ConstStr(std::string s)
    {
        var = s.c_str();
    }
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

private:
    const char *var;
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

private:
    LValue *left;
    Expr *expr;
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
        globals[l_value->getName()] = expr->eval();
    }

private:
    LValue *l_value;
    Expr *expr;
};

class Call : public Expr
{
public:
    Call(std::string s) : id(s) {}
    Call(std::string s, ExprList *e_list) : id(s), expr_list(e_list) {}
    ~Call()
    {
        delete expr_list;
    }
    virtual void printOn(std::ostream &out) const override
    {
        bool first = true;
        out << "Call( " << id << " ( " << expr_list << "))";
    }
    // TODO: Implement how a function is run.

private:
    std::string id;
    //std::vector<Expr *> expr_list;
    ExprList *expr_list;
};

class UnOp : public Expr
{
public:
    UnOp(std::string s, Expr *e) : op(s), right(e) {}
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
    Expr *right;
    std::string op;
};

class BinOp : public Expr
{
public:
    BinOp(Expr *l, std::string s, Expr *r) : left(l), op(s), right(r) {}
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
    Expr *left, *right;
    std::string op;
};

class OpCond : public Cond
{
public:
    OpCond(Cond *l, std::string s, Cond *r) : left(l), op(s), right(r) {}
    OpCond(std::string s, Cond *r) : left(), op(s), right(r) {}
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
    Cond *left, *right;
    std::string op;
};

class Return : public Stmt
{
public:
    Return(Expr *e = nullptr) : expr(e) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Return(";
        if (expr != nullptr)
            out << expr;
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

class Block : public AST
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
        for (Stmt *s : stmt_list)
        {
            if (!first)
                out << ",";
            first = false;
            out << *s;
        }
        out << ")";
    }
    // virtual void run() const override
    // {
    //     for (Stmt *s : stmt_list)
    //         s->run();
    // }
    std::vector<Stmt *> getList()
    {
        return stmt_list;
    }

private:
    std::vector<Stmt *> stmt_list;
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
            out << ", " << *stmt2;
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

class DataType : public AST
{
public:
    DataType(std::string t) : type(t) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "DataType(" << type;
    }

private:
    std::string type;
};

class RetType : public AST
{
public:
    RetType(DataType *t) : type(t) {}
    ~RetType() { delete type; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "RetType(";
        if (type == nullptr)
            out << "nothing)";
        else
            out << *type << ")";
    }

private:
    DataType *type;
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
        out << "FParType(";
        if (dimlen)
            out << "[ ]";
        out << dim << ")";
    }
private:
    DataType *type;
    ArrayDim *dim;
    bool dimlen;
};

#endif __AST_HPP__