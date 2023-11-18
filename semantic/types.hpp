#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ast.hpp"

// enum Types
// {
//     TYPE_INTEGER,
//     TYPE_BOOLEAN,
//     TYPE_CHARACTER,
//     TYPE_STRING,
//     TYPE_ARRAY,
//     TYPE_NOTHING
// };

class Types : public AST
{
public:
    virtual ~Types() {}
    virtual void printOn(std::ostream &out) const = 0;

    // protected:
    //     Types Types;
};

class Integer : public Types
{
public:
    Integer() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "int";
    }
};

class Boolean : public Types
{
public:
    Boolean() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "bool";
    }
};

class Character : public Types
{
public:
    Character() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "char";
    }
};

class Str : public Types
{
public:
    Str() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "string";
    }
};

class Array : public Types
{

public:
    Array(Integer *i) : integer(i), character(nullptr) {}
    Array(Character *t) : integer(nullptr), character(t) {}
    ~Array()
    {
        delete integer;
        delete character;
    }
    virtual void printOn(std::ostream &out) const override
    {
        if (integer != nullptr)
        {
            out << *integer;
        }
        else
        {
            out << *character;
        }
        out << " array";
    }

private:
    Integer *integer;
    Character *character;
};

class Nothing : public Types
{
public:
    Nothing() {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "nothing";
    }
};

#endif