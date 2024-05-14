#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <vector>
#include <map>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Function.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

/* ---------------------------------------------------------------------
   --------------------------- Symbol Table ----------------------------
   --------------------------------------------------------------------- */

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
class Parameter
{
public:
    Parameter(const char *n, Type t, PassMode pm) : name(n), type(t), pass_mode(pm) {}
    const char *getName() const { return name; }
    Type getType() const { return type; }
    PassMode getPassMode() const { return pass_mode; }

private:
    const char *name;
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

typedef llvm::Type llvmType;

class AST
{
public:
    AST();
    virtual ~AST() = default;
    virtual void printOn(std::ostream &out) const = 0;
    virtual void sem() {};
    void SemanticError(const char *msg);

    llvm::Value *LogErrorV(const char *Str) const;
    virtual llvm::Value *compile() const {};

    void llvm_compile_and_dump();

    // Functions for library functions
    void llvmAddLibraryFunction(const char *func_name, const std::vector<llvmType *> params_type, llvmType *return_type);
    void llvmAddLibrary();

protected:
    // Global LLVM variables related to the LLVM suite.
    static llvm::LLVMContext TheContext;
    static llvm::IRBuilder<> Builder;
    static std::unique_ptr<llvm::Module> TheModule;
    static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;

    // Useful LLVM types.
    static llvm::Type *i8;
    static llvm::Type *i32;
    static llvm::Type *i64;
    static llvm::Type *voidTy;

    // Useful LLVM helper functions.
    llvm::ConstantInt *c8(char c) const
    {
        return llvm::ConstantInt::get(TheContext, llvm::APInt(8, c, true));
    }
    llvm::ConstantInt *c32(int n) const
    {
        return llvm::ConstantInt::get(TheContext, llvm::APInt(32, n, true));
    }
    llvm::ConstantInt *c64(int n) const
    {
        return llvm::ConstantInt::get(TheContext, llvm::APInt(64, n, true));
    }

    static std::map<std::string, llvm::AllocaInst *> NamedValues;

    /// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
    /// the function.  This is used for mutable variables etc.
    static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::StringRef VarName, llvmType *llvm_type);

    int lineno;
};

llvmType *getLLVMType(Type t, llvm::LLVMContext& context);

inline std::vector<bool> returnedFunction;

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
};

#endif
