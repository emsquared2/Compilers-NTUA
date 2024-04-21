#ifndef __ASTABSTRACTS_HPP__
#define __ASTABSTRACTS_HPP__

#include <iostream>

/* Link C with C++ */
extern "C" {
    #include "symbol.h"

    extern void          initSymbolTable    (unsigned int size);
    extern void          destroySymbolTable (void);

    extern void          openScope          (void);
    extern void          closeScope         (void);

    extern SymbolEntry * newVariable        (const char * name, Type type);
    extern SymbolEntry * newConstant        (const char * name, Type type, ...);
    extern SymbolEntry * newFunction        (const char * name);
    extern SymbolEntry * newParameter       (const char * name, Type type,
                                     PassMode mode, SymbolEntry * f);
    extern SymbolEntry * newTemporary       (Type type);

    extern void          forwardFunction    (SymbolEntry * f);
    extern void          endFunctionHeader  (SymbolEntry * f, Type type);
    extern void          destroyEntry       (SymbolEntry * e);
    extern SymbolEntry * lookupEntry        (const char * name, LookupType type,
                                     bool err);

    extern Type          typeArray          (RepInteger size, Type refType);
    extern Type          typeIArray         (Type refType);
    extern Type          typePointer        (Type refType);
    extern void          destroyType        (Type type);
    extern unsigned int  sizeOfType         (Type type);
    extern bool          equalType          (Type type1, Type type2);
    extern void          printType          (Type type);
    extern void          printMode          (PassMode mode);

    extern void          destroyType        (Type type);
}


/* ---------------------------------------------------------------------
   ------------------------------ DataType -----------------------------
   --------------------------------------------------------------------- */


enum DataType { TYPE_INTEGER, TYPE_CHAR, TYPE_VOID, TYPE_ARRAY, TYPE_IARRAY };


/* ---------------------------------------------------------------------
   -------------------------------- AST --------------------------------
   --------------------------------------------------------------------- */

class AST
{
public:
    AST();
    virtual ~AST() {}
    virtual void printOn(std::ostream &out) const = 0;
    virtual void sem() {}

    // TODO add int lineno as protected field and print it when detecting a semantic error.
    void SemanticError(const char *msg) {
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }
};

/* ---------------------------------------------------------------------
   ------------------------------- Expr --------------------------------
   --------------------------------------------------------------------- */

class Expr : public AST
{
public:
    // virtual int eval() const = 0;
    Type getType() { return type; }

    void type_check(Type t)
    {
        sem();

        if (!equalType(type, t))
        {
            SemanticError("Type mismatch");
        }
    }

protected:
    Type type;
};

/* ---------------------------------------------------------------------
   ------------------------------- Cond --------------------------------
   --------------------------------------------------------------------- */

class Cond : public AST
{
public:
    // virtual int eval() const = 0;
    Type getType() { return type; }

    void type_check(Type t)
    {
        sem();
        if (!equalType(type, t))
        {
            SemanticError("Type mismatch");
        }
    }

protected:
    Type type;
};


/* ---------------------------------------------------------------------
   ------------------------------- Stmt --------------------------------
   --------------------------------------------------------------------- */

class Stmt : public AST
{
public:
    virtual void run() const = 0;
};


/* ---------------------------------------------------------------------
   ------------------------------- LValue ------------------------------
   --------------------------------------------------------------------- */

class LValue : public Expr
{
public:
    virtual int eval() const = 0;
    virtual const char * getName() const {};
};

/* ---------------------------------------------------------------------
   ---------------------------- Custom Type ----------------------------
   --------------------------------------------------------------------- */

class CustomType : public AST
{
public:
    virtual void printOn(std::ostream &out) const = 0;
    virtual Type ConvertToType(CustomType *t) const = 0;
};

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
};

#endif
