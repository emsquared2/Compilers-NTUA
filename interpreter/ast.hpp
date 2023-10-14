#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <map>
#include <string>
#include <vector>

extern std::map<std::string, int> globals;

struct ExprList
{
    std::vector<Expr *> expr_list;
};

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
    virtual char eval() const override
    {
        return var;
    }

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
    Call(std::string s, Expr *e, Block<Expr> e_list) : id(s)
    {
        expr_list.push_back(e);
        for (Expr *e : e_list.getList())
        {
            expr_list.push_back(e);
        }
    }
    ~Call()
    {
        for (Expr *e : expr_list)
        {
            delete e;
        }
        expr_list.clear();
    }
    virtual void printOn(std::ostream &out) const override
    {
        bool first = true;
        out << "Call( " << id << " ( ";
        for (Expr *e : expr_list)
        {
            if (!first)
                out << ", ";
            out << *e;
            first = false;
        }
        out << " ))";
    }
    // TODO: Implement how a function is run.

private:
    std::string id;
    std::vector<Expr *> expr_list;
};

class UnOp : public Expr 
{
public:
    UnOp(std::string s, Expr *e): op(s), right(e) {}
    ~UnOp() {
        delete right;
    }
    virtual void printOn(std::ostream &out) const override 
    {
        out << op << "(" << *right << ")"; 
    }
    virtual int eval() const override 
    {
        if(op == "+")
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
    OpCond(std::string s, Cond *r) : op(s), right(r) {}
    ~OpCond()
    {
        delete left;
        delete right;
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

template <class T>
class Block /*: public Stmt */
{
public:
    Block() : stmt_list() {}
    ~Block()
    {
        for (T *s : stmt_list)
            delete s;
    }
    void append(T *s) { stmt_list.push_back(s); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Block(";
        bool first = true;
        for (T *s : stmt_list)
        {
            if (!first)
                out << ",";
            first = false;
            out << *s;
        }
        out << ")";
    }
    virtual void run() const override
    {
        for (T *s : stmt_list)
            s->run();
    }
    std::vector<T *> getList()
    {
        return stmt_list;
    }

private:
    std::vector<T *> stmt_list;
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

#endif __AST_HPP__