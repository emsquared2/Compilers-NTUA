#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ASTabstracts.hpp"

/*
############################
        BASE TYPES
############################
*/
class Integer : public Type
{
public:
    Integer() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "int";
    }
};

class Boolean : public Type
{
public:
    Boolean() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "bool";
    }
};

class Character : public Type
{
public:
    Character() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "char";
    }
};

class Str : public Type
{
public:
    Str() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "string";
    }
};

class Nothing : public Type
{
public:
    Nothing() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "nothing";
    }
};

/*
############################
        CONST TYPES
############################
*/

class Const : public Expr
{
public:
    Const(int n) : num(n) { type = new Integer(); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "Const(" << num << ")";
    }
    virtual int eval() const override
    {
        return num;
    }
    virtual void sem() override {}

private:
    int num;
};

class ConstChar : public Expr
{
public:
    ConstChar(char v) : var(v) { type = new Character(); }
    virtual void printOn(std::ostream &out) const override
    {
        out << "ConstChar(" << var << ")";
    }
    virtual int eval() const override
    {
        // return var;
    }
    virtual void sem() override {}

private:
    char var;
};

class ConstStr : public LValue
{
public:
    ConstStr(char *s) : var(s) { type = new Str(); }
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
    virtual void sem() override {}

private:
    const char *var;
};

/*
############################
        ARRAY TYPE
############################
*/

/// @brief helper function for array dimension
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
        if (unknown)
            out << "[ ]";
        for (auto num = dim.rbegin(); num != dim.rend(); ++num)
        {
            out << "[" << **num << "]";
        }
    }
    bool isEmpty()
    {
        return dim.empty();
    }
    void specifyUnknownFirstDim()
    {
        unknown = true;
    }
    void append(Const *num)
    {
        dim.push_back(num);
    }
    // TODO: for semantic action check each num that is of the correct type
private:
    std::vector<Const *> dim;
    bool unknown = false;
};

class Array : public Type
{

public:
    Array(Type *t, ArrayDim *d) : type(t), dim(d) {}
    ~Array()
    {
        delete type;
        delete dim;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << *type << " array" << *dim;
    }

private:
    Type *type;
    ArrayDim *dim;
};


/// @brief return type of function
class RetType : public Type
{
public:
    RetType() { dtype = new Nothing(); }
    RetType(Type *t) : dtype(t) {}
    ~RetType() { delete dtype; }
    virtual void printOn(std::ostream &out) const override
    {
        out << "RetType(" << *dtype << ")";
    }

private:
    Type *dtype;
};

/// @brief function parameter type
class FParType : public Type
{
public:
    FParType(Type *t) : type(t) {}
    ~FParType()
    {
        delete type;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParType(" << *type << ")";
    }

private:
    Type *type;
};

#endif
