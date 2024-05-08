#include "ast.hpp"

extern int yylineno;

AST::AST() : lineno(yylineno){};

void AST::SemanticError(const char *msg)
{
    fprintf(stderr, "Error: %s at line %d\n", msg, lineno);
    // fprintf(stderr, "%s\n", msg);
    exit(1);
}

void AST::llvm_compile_and_dump()
{
    TheModule = std::make_unique<llvm::Module>("grace program", TheContext);
    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

    // add optimization functions

    TheFPM->doInitialization();

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