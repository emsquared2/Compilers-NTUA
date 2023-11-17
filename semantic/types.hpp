#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ast.hpp"

enum Types { TYPE_INTEGER, TYPE_BOOLEAN, TYPE_CHARACTER, TYPE_STRING, TYPE_ARRAY, TYPE_NOTHING };

class Type : public AST
{
public:
    virtual ~Type() {}
    virtual void printOn(std::ostream &out) const = 0;

protected:
    Types *type;
};

class Integer : public Type
{
public:
    Integer(int v) : val(v), type(TYPE_INTEGER) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "int";
    }

private:
    int val;
};

class Boolean : public Type
{
public:
    Boolean(int b) : boolean(b), type(TYPE_BOOLEAN) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "bool";
    }

private:
    int boolean;
};

class Character : public Type
{
public:
    Character(char c) : chr(c), type(TYPE_CHARACTER) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "char";
    }

private:
    char chr;
};

class Str : public Type
{
public:
    Str(char *s) : str(s), type(TYPE_STRING) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "string";
    }

private:
    char *str;
};

class Array : public Type
{

public:
    Array(Integer *i) : integer(i), character(nullptr), type(TYPE_ARRAY) {}
    Array(Character *t) : integer(nullptr), character(t), type(TYPE_ARRAY) {}
    ~Array() {
        delete integer;
        delete character;
    }
    virtual void printOn(std::ostream &out) const override
    {
        if(integer != nullptr) {
            out << *integer;
        }
        else {
            out << *character;
        }
        out << " array";
    }

private:
    Integer *integer;
    Character *character;
};

class Nothing : public Type
{
    Nothing() : type(TYPE_NOTHING) {}
    virtual void printOn(std::ostream &out) const override
    {
        out << "nothing";
    }
}

#endif