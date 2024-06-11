#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Function.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/TypeBasedAliasAnalysis.h"
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Utils.h>

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Host.h"

#include "llvm/IR/DataLayout.h"
#include "llvm/Support/ErrorHandling.h"

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
    // Base methods
    AST();
    virtual ~AST() = default;
    virtual void printOn(std::ostream &out) const = 0;
    virtual void sem() {};
    void SemanticError(const char *msg);
    llvm::Value *LogErrorV(const char *Str) const;
    virtual llvm::Value *compile() { return nullptr; }
    virtual llvm::Value *compile_ptr() { return nullptr; }
    virtual llvm::Value * compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t) { return nullptr; }

    // Functions for library functions
    void llvmAddLibraryFunction(const char *func_name, const std::vector<llvmType *> params_type, llvmType *return_type);
    void llvmAddLibrary();
    
    // Function for initialization
    void llvm_compile_and_dump();

    // Function for optimizations
    void FPM_Optimizations();

    // Functions for Code Generation
    llvm::Function *MainCodeGen(llvm::Value* main_function);
    void emitLLVMIR(const std::string& outputTarget);
    void emitAssembly(const std::string & outputTarget);


    static std::set<std::string> CapturedVariables;
    static std::map<std::string, unsigned int> FunctionDepth;

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


/* ---------------------------------------------------------------------
   ------------------------------- UTILS -------------------------------
   --------------------------------------------------------------------- */

template <typename T>
std::vector<T> getReversed(const std::vector<T>& input)
{
    std::vector<T> reversed_vector(input.rbegin(), input.rend());
    return reversed_vector;
}

std::string getMangledName(const char * name, int scope);

llvmType *getLLVMType(Type t, llvm::LLVMContext& context);

inline std::vector<bool> returnedFunction;

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
    t.printOn(out);
    return out;
};

// Flags and filename
extern bool optimize, genFinal, genIntermediate;
extern std::string filename;

#endif
