#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ast.hpp"

class Type : public AST
{
public:
    virtual ~Type() {}
    virtual void printOn(std::ostream &out) const = 0;
};

class Integer : public Type
{
public:
    Integer(int v) : val(v) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Int(" << val << ")";
    }

private:
    int val;
};

class Boolean : public Type
{
public:
    Boolean(int b) : boolean(b) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Boolean(";
        switch (boolean)
        {
        case 0:
            out << "false";
            break;
        case 1:
            out << "true";
            break;
        default:
        }
        out << ")";
    }

private:
    int boolean;
};

class Character : public Type
{
public:
    Character(char c) : chr(c) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "Char(" << chr << ")";
    }

private:
    char chr;
};

class Str : public Type
{
public:
    Str(char *s) : str(s) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "String(" << *str << ")";
    }

private:
    char *str;
};

class Array : public Type
{

public:
    Array(char cArray[])
    {
        charArray = cArray;
        intArray = nullptr;
    }
    Array(int iArray[])
    {
        intArray = iArray;
        charArray = nullptr;
    }
    virtual void printOn(std::ostream &out) const override
    {
        if (intArray != nullptr)
        {
            out << "IntArray(";
            for (int i = 0; intArray[i] != '\0'; ++i)
            {
                out << intArray[i] << " ";
            }
        }
        else if (charArray != nullptr)
        {
            out << "CharArray(" << charArray;
        }
        out << ")";
    }

private:
    char *charArray;
    int *intArray;
};

#endif