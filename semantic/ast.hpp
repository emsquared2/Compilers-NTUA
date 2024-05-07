#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <vector>

/* Link C with C++ */
extern "C"
{
#include "symbol.h"

    extern void initSymbolTable(unsigned int size);
    extern void destroySymbolTable(void);

    extern void openScope(void);
    extern void closeScope(void);

    extern SymbolEntry *newVariable(const char *name, Type type);
    extern SymbolEntry *newConstant(const char *name, Type type, ...);
    extern SymbolEntry *newFunction(const char *name);
    extern SymbolEntry *newParameter(const char *name, Type type,
                                     PassMode mode, SymbolEntry *f);
    extern SymbolEntry *newTemporary(Type type);

    extern void forwardFunction(SymbolEntry *f);
    extern void endFunctionHeader(SymbolEntry *f, Type type);
    extern void destroyEntry(SymbolEntry *e);
    extern SymbolEntry *lookupEntry(const char *name, LookupType type, bool err);

    extern Type typeArray(RepInteger size, Type refType);
    extern Type typeIArray(Type refType);
    extern Type typePointer(Type refType);
    extern void destroyType(Type type);
    extern unsigned int sizeOfType(Type type);
    extern bool equalType(Type type1, Type type2);
    void NewFunction(Type type1, Type type2);
    extern void printType(Type type);
    extern void printMode(PassMode mode);

    extern void destroyType(Type type);
}


/* ---------------------------------------------------------------------
   ----------------------------- Parameter -----------------------------
   --------------------------------------------------------------------- */

// This class represents a function parameter and is used in addLibrary and addLibraryFunction.
class Parameter {
public:
    Parameter(const char* n, Type t, PassMode pm) : name(n), type(t), pass_mode(pm) {}
    const char* getName() const { return name; }
    Type getType() const { return type; }
    PassMode getPassMode() const { return pass_mode; }

private:
    const char* name;
    Type type;
    PassMode pass_mode;
};

/* ---------------------------------------------------------------------
   ----------------------- Add Library Functions -----------------------
   --------------------------------------------------------------------- */

void addLibrary();

/* ---------------------------------------------------------------------
   ------------------------------ DataType -----------------------------
   --------------------------------------------------------------------- */

enum DataType
{
    TYPE_INTEGER,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_ARRAY,
    TYPE_IARRAY
};

/* ---------------------------------------------------------------------
   -------------------------------- AST --------------------------------
   --------------------------------------------------------------------- */

class AST
{
public:
    AST();
    virtual ~AST() = default;
    virtual void printOn(std::ostream &out) const = 0;
    virtual void sem(){};
    void SemanticError(const char *msg);

protected:
    int lineno;
};


inline std::vector<bool> returnedFunction;
 
inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
};

#endif
