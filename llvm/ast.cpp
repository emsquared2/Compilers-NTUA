#include "ast.hpp"

extern int yylineno;

AST::AST() : lineno(yylineno){};

void AST::SemanticError(const char *msg)
{
    fprintf(stderr, "Error: %s at line %d\n", msg, lineno);
    // fprintf(stderr, "%s\n", msg);
    exit(1);
}

/* ---------------------------------------------------------------------
   ----------------------- Add Library Functions -----------------------
   --------------------------------------------------------------------- */

void addLibraryFunction(const char *func_name, const std::vector<Parameter *> &params, Type return_type)
{
    SymbolEntry *f = newFunction(func_name);
    openScope();

    for (const auto &param : params)
    {
        newParameter(param->getName(), param->getType(), param->getPassMode(), f);
    }

    endFunctionHeader(f, return_type);
    closeScope();
}

void addLibrary()
{

    std::vector<Parameter *> params;
    Type string = typeIArray(typeChar);
    // Type string = typeArray(???, typeChar);

    /* ---------------------------------
    --------- Output Functions ---------
    ------------------------------------ */

    // writeInteger

    params.push_back(new Parameter("n", typeInteger, PASS_BY_VALUE));
    addLibraryFunction("writeInteger", params, typeVoid);
    params.clear();

    // writeChar

    params.push_back(new Parameter("c", typeChar, PASS_BY_VALUE));
    addLibraryFunction("writeChar", params, typeVoid);
    params.clear();

    // writeString

    params.push_back(new Parameter("s", string, PASS_BY_REFERENCE));
    addLibraryFunction("writeString", params, typeVoid);
    params.clear();

    /* ---------------------------------
    --------- Input Functions ----------
    ------------------------------------ */

    // readInteger

    addLibraryFunction("readInteger", params, typeInteger);

    // readChar

    addLibraryFunction("readChar", params, typeChar);

    // readString

    params.push_back(new Parameter("n", typeInteger, PASS_BY_VALUE));
    params.push_back(new Parameter("s", string, PASS_BY_REFERENCE));
    addLibraryFunction("readString", params, typeVoid);
    params.clear();

    /* ---------------------------------
     ------- Conversion Functions ------
     ------------------------------------ */

    // ascii

    params.push_back(new Parameter("c", typeChar, PASS_BY_VALUE));
    addLibraryFunction("ascii", params, typeInteger);
    params.clear();

    // chr

    params.push_back(new Parameter("n", typeInteger, PASS_BY_VALUE));
    addLibraryFunction("chr", params, typeChar);
    params.clear();

    /* ---------------------------------
    ---------- String Functions --------
    ------------------------------------ */

    // strlen

    params.push_back(new Parameter("s", string, PASS_BY_REFERENCE));
    addLibraryFunction("strlen", params, typeInteger);
    params.clear();

    // strcmp

    params.push_back(new Parameter("s1", string, PASS_BY_REFERENCE));
    params.push_back(new Parameter("s2", string, PASS_BY_REFERENCE));
    addLibraryFunction("strcmp", params, typeInteger);
    params.clear();

    // strcpy

    params.push_back(new Parameter("trg", string, PASS_BY_REFERENCE));
    params.push_back(new Parameter("src", string, PASS_BY_REFERENCE));
    addLibraryFunction("strcpy", params, typeVoid);
    params.clear();

    // strcat

    params.push_back(new Parameter("trg", string, PASS_BY_REFERENCE));
    params.push_back(new Parameter("src", string, PASS_BY_REFERENCE));
    addLibraryFunction("strcat", params, typeVoid);
    params.clear();

    destroyType(string);
}

/* ---------------------------------------------------------------------
   ------------------- Add Library Functions - LLVM --------------------
   --------------------------------------------------------------------- */

void AST::llvmAddLibraryFunction(const char * func_name, const std::vector<llvmType*> params_type, llvmType* return_type)
{
  llvm::FunctionType *t = llvm::FunctionType::get(return_type, params_type, false);
  llvm::Function::Create(t, llvm::Function::ExternalLinkage, func_name, TheModule.get());
}

void AST::llvmAddLibrary() {

    /* ---------------------------------
    --------- Output Functions ---------
    ------------------------------------ */

    llvmAddLibraryFunction("writeInteger", std::vector<llvmType*>{i64}, voidTy);
    llvmAddLibraryFunction("writeChar", std::vector<llvmType*>{i8}, voidTy);
    llvmAddLibraryFunction("writeString", std::vector<llvmType*>{llvm::PointerType::get(i8, 0)}, voidTy);

    /* ---------------------------------
    --------- Input Functions ----------
    ------------------------------------ */

    llvmAddLibraryFunction("readInteger", std::vector<llvmType*>{}, i64);
    llvmAddLibraryFunction("readChar", std::vector<llvmType*>{}, i8);
    llvmAddLibraryFunction("readString", std::vector<llvmType*>{i64, llvm::PointerType::get(i8, 0)}, voidTy);

    /* ---------------------------------
    ------ Conversion Functions -------
    ------------------------------------ */

    llvmAddLibraryFunction("ascii", std::vector<llvmType*>{i8}, i64);
    llvmAddLibraryFunction("chr", std::vector<llvmType*>{i64}, i8);

    /* ---------------------------------
    ---------- String Functions --------
    ------------------------------------ */

    llvmAddLibraryFunction("strlen", std::vector<llvmType*>{llvm::PointerType::get(i8, 0)}, i64);
    llvmAddLibraryFunction("strcmp", std::vector<llvmType*>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, i64);
    llvmAddLibraryFunction("strcpy", std::vector<llvmType*>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, voidTy);
    llvmAddLibraryFunction("strcat", std::vector<llvmType*>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, voidTy);
}

/* ---------------------------------------------------------------------
   ----------------------- LLVM Compile and Dump -----------------------
   --------------------------------------------------------------------- */


void AST::llvm_compile_and_dump()
{
    
    TheModule = std::make_unique<llvm::Module>("grace program", TheContext);

    // Add Library Functions
    llvmAddLibrary();

    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

    // add optimization functions

    TheFPM->doInitialization();

    // Create and add entry point for main function
    llvm::FunctionType *funcType = llvm::FunctionType::get(i32, false); // false indicates the function does not take variadic arguments.
    llvm::Function *main = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", TheModule.get());

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", main);
    Builder.SetInsertPoint(BB);
    // Emit the program code.
    compile();
    Builder.CreateRet(c32(0));
    
    // Verify the IR.
    bool bad = verifyModule(*TheModule, &llvm::errs());
    if (bad)
    {
        std::cerr << "The IR is bad!" << std::endl;
        std::exit(1);
    }
    TheFPM->run(*main);
    // Print out the IR.
    TheModule->print(llvm::outs(), nullptr);
}

/* ---------------------------------------------------------------------
   ----------------------------- LogErrorV -----------------------------
   --------------------------------------------------------------------- */

llvm::Value* AST::LogErrorV(const char *Str) const
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

/* ---------------------------------------------------------------------
   ---------------------------- getLLVMType ----------------------------
   --------------------------------------------------------------------- */


llvmType * AST::getLLVMType(Type t) {
    if (equalType(t, typeVoid))
        return llvmType::getVoidTy(TheContext);
    else if (equalType(t, typeInteger))
        return llvmType::getInt32Ty(TheContext);
    else if (equalType(t, typeChar))
        return llvmType::getInt8Ty(TheContext);
    else {
        llvmType *elementType = getLLVMType(t->refType);
        // Array 
        if (t->size > 0) {
            return llvm::ArrayType::get(elementType, t->size);
        }
        else
            return elementType;
    }
}