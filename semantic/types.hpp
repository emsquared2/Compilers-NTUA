#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#pragma once

class Types : public AST
{
public:
    virtual ~Types() {}
    virtual void printOn(std::ostream &out) const = 0;
};

class Integer : public Types
{
public:
    Integer();
    virtual void printOn(std::ostream &out) const override;
};

class Boolean : public Types
{
public:
    Boolean();
    virtual void printOn(std::ostream &out) const override;
};

class Character : public Types
{
public:
    Character();
    virtual void printOn(std::ostream &out) const override;
};

class Str : public Types
{
public:
    Str();
    virtual void printOn(std::ostream &out) const override;
};

class Array : public Types
{

public:
    Array(Types *t, ArrayDim *d);
    ~Array();
    virtual void printOn(std::ostream &out) const override;

private:
    Types *type;
    ArrayDim *dim;
};

class Nothing : public Types
{
public:
    Nothing();
    virtual void printOn(std::ostream &out) const override;
};

#endif