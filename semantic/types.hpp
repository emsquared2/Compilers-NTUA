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
        out << "int";
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
        out << "bool";
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
        out << "char";
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
        out << "string";
    }

private:
    char *str;
};

class Array : public Type
{

public:
    Array(Integer *i) : integer(i), character(nullptr) {}
    Array(Character *t) : integer(nullptr), character(t) {}
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
    }

private:
    Integer *integer;
    Character *character;
};

#endif