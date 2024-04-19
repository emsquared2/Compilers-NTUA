#pragma once

#include <cstdlib>
#include <vector>
// #include "ast.hpp"
// #include "types.hpp"
#include <string>
#include <map>

class SymbolEntry
{
public:
    SymbolEntry() {}
    SymbolEntry(Type *t, int ofs) : type(t), offset(ofs) {}

    Type *getType() const { return type; }
    int getOffset() const { return offset; }

private:
    Type *type;
    int offset;
};

class Scope
{
public:
    Scope() : locals(), offset(-1), size(0) {}
    Scope(int ofs = 0) : locals(), offset(ofs), size(0) {}

    int getOffset() const { return offset; }
    int getSize() const { return size; }

    SymbolEntry *lookup(const std::string &name)
    {
        if (locals.find(name) == locals.end())
            return nullptr;
        return &(locals[name]);
    }

    void insert(const std::string name, Type *t)
    {
        if (locals.find(name) != locals.end())
        {
            std::cerr << "Duplicate variable " << name << std::endl;
            exit(1);
        }
        locals[name] = SymbolEntry(t, offset++);
        ++size;
    }

private:
    std::map<std::string, SymbolEntry> locals;
    int offset;
    int size;
};

class SymbolTable
{
public:
    void openScope()
    {
        int ofs = scopes.empty() ? 0 : scopes.back().getOffset();
        scopes.push_back(Scope(ofs));
    }
    void closeScope() { scopes.pop_back(); };

    SymbolEntry *lookup(std::string name)
    {
        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i)
        {
            SymbolEntry *e = i->lookup(name);
            if (e != nullptr)
                return e;
        }
        std::cerr << "Unknown variable " << name << std::endl;
        exit(1);
    }

    int getSizeOfCurrentScope() const { return scopes.back().getSize(); }

    void insert(std::string str, Type *t) { scopes.back().insert(str, t); }

private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;