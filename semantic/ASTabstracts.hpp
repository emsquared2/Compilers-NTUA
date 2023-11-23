#ifndef __ASTABSTRACTS_HPP__
#define __ASTABSTRACTS_HPP__

#include <iostream>

class AST
{
public:
    virtual ~AST() {}
    virtual void printOn(std::ostream &out) const = 0;
    virtual void sem() {}
};

class Type : public AST
{
public:
    virtual ~Type() {}
    virtual void printOn(std::ostream &out) const = 0;
};

class Expr : public AST
{
public:
    virtual int eval() const = 0;
    void type_check(Type *t)
    {
        sem();
        if(type != t) {
            std::cerr << "Type mismatch" << std::endl;
            exit(1);
        }
    }

protected:
    Type *type;
};

class Cond : public AST
{
public:
    virtual int eval() const = 0;
    // might not be needed 
    void type_check(Type *t)
    {
        sem();
        if (type != t)
        {
            std::cerr << "Type mismatch" << std::endl;
            exit(1);
        }
    }
protected:
    Type *type;
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

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
};

#endif