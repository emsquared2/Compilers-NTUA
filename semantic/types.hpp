#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ASTBase.hpp"

class Type : public AST
{
public:
    virtual ~Type() {}
    virtual void printOn(std::ostream &out) const = 0;
};

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

#endif
