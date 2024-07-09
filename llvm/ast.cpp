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
   ------------------- Add Library Functions - SEM  --------------------
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

    AST::FunctionDepth[func_name] = currentScope->nestingLevel;
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

void AST::llvmAddLibraryFunction(const char *func_name, const std::vector<llvmType *> params_type, llvmType *return_type)
{
    llvm::FunctionType *t = llvm::FunctionType::get(return_type, params_type, false);
    llvm::Function::Create(t, llvm::Function::ExternalLinkage, func_name, TheModule.get());
}

void AST::llvmAddLibrary()
{
    /* ---------------------------------
    --------- Output Functions ---------
    ------------------------------------ */

    llvmAddLibraryFunction("writeInteger", std::vector<llvmType *>{i64}, voidTy);
    llvmAddLibraryFunction("writeChar", std::vector<llvmType *>{i8}, voidTy);
    llvmAddLibraryFunction("writeString", std::vector<llvmType *>{llvm::PointerType::get(i8, 0)}, voidTy);

    /* ---------------------------------
    --------- Input Functions ----------
    ------------------------------------ */

    llvmAddLibraryFunction("readInteger", std::vector<llvmType *>{}, i64);
    llvmAddLibraryFunction("readChar", std::vector<llvmType *>{}, i8);
    llvmAddLibraryFunction("readString", std::vector<llvmType *>{i64, llvm::PointerType::get(i8, 0)}, voidTy);

    /* ---------------------------------
    ------ Conversion Functions -------
    ------------------------------------ */

    llvmAddLibraryFunction("ascii", std::vector<llvmType *>{i8}, i64);
    llvmAddLibraryFunction("chr", std::vector<llvmType *>{i64}, i8);

    /* ---------------------------------
    ---------- String Functions --------
    ------------------------------------ */

    llvmAddLibraryFunction("strlen", std::vector<llvmType *>{llvm::PointerType::get(i8, 0)}, i64);
    llvmAddLibraryFunction("strcmp", std::vector<llvmType *>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, i64);
    llvmAddLibraryFunction("strcpy", std::vector<llvmType *>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, voidTy);
    llvmAddLibraryFunction("strcat", std::vector<llvmType *>{llvm::PointerType::get(i8, 0), llvm::PointerType::get(i8, 0)}, voidTy);
}

/* ---------------------------------------------------------------------
   ------------------------- FPM Optimizations -------------------------
   --------------------------------------------------------------------- */

void AST::FPM_Optimizations()
{
    // Initialize the function pass manager with the current LLVM module.
    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

    if (optimize)
    {
        // Add type-based alias analysis pass which provides information about pointer aliasing.
        TheFPM->add(llvm::createTypeBasedAAWrapperPass());
        // Add basic alias analysis pass providing less precise but fast aliasing information.
        TheFPM->add(llvm::createBasicAAWrapperPass());
        
        // Simplify the control flow graph (e.g., remove unreachable blocks).
        TheFPM->add(llvm::createCFGSimplificationPass());
        // Perform Scalar Replacement of Aggregates (breaks aggregates into individual scalars).
        TheFPM->add(llvm::createSROAPass());
        // Promote 'alloca' memory to register memory where possible.
        TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
        // Perform early Common Subexpression Elimination, removing redundant expressions.
        TheFPM->add(llvm::createEarlyCSEPass());
        // Perform simple peephole optimizations and bit-twiddling optzns.
        TheFPM->add(llvm::createInstructionCombiningPass());
        // Reassociate expressions to allow for better constant propagation.
        TheFPM->add(llvm::createReassociatePass());
        // Eliminate common subexpressions, enhancing the efficiency of generated code.
        TheFPM->add(llvm::createGVNPass());
        // Dead Code Elimination: Remove unused code that does not affect program output.
        TheFPM->add(llvm::createDeadCodeEliminationPass());
        // Further simplify CFG, removing dead blocks and merging blocks.
        TheFPM->add(llvm::createCFGSimplificationPass());
    }
    TheFPM->doInitialization();
}

/* ---------------------------------------------------------------------
   --------------------------- Main CodeGen ----------------------------
   --------------------------------------------------------------------- */

llvm::Function * AST::MainCodeGen(llvm::Value* main_function)
{   
    // Create and add entry point for main function
    llvm::FunctionType *funcType = llvm::FunctionType::get(i64, {}, false); // false indicates the function does not take variadic arguments.
    llvm::Function *main = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", TheModule.get());
    
    // Create the basic block for the main function
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", main);
    Builder.SetInsertPoint(BB);

    Builder.CreateCall(llvm::dyn_cast<llvm::Function>(main_function));
    Builder.CreateRet(c64(0));
    
    return main;
}

/* ---------------------------------------------------------------------
   ----------------------------- emitLLVMIR ----------------------------
   --------------------------------------------------------------------- */

/**
 * Generates the LLVM Intermediate Representation (IR) for the current module
 * and outputs it to a file specified by the `Filename`.
 *
 * @param Filename: The intended output filename (implicitly used as 'Filename').
 */
void AST::emitLLVMIR(const std::string& Filename)
{
    std::error_code EC;
    llvm::raw_fd_ostream oss(Filename, EC);
    TheModule->print(oss, nullptr);
}

/* ---------------------------------------------------------------------
   ---------------------------- emitAssembly ---------------------------
   --------------------------------------------------------------------- */

/**
 * Generates the assembly code for the current module according to a specified
 * target architecture and outputs it to an object file.
 *
 * @param Filename: The target specification for generating the assembly code.
 */
void AST::emitAssembly(const std::string & Filename)
{
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    // Initialize the target registry etc.
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        llvm::errs() << Error;
        exit(1);
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TheTargetMachine = Target->createTargetMachine(
        TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    if (EC) {
       llvm::errs() << "Could not open file: " << EC.message();
        exit(1);
    }

    llvm::legacy::PassManager pass;
    // auto FileType = llvm::CodeGenFileType::ObjectFile;
    auto FileType = llvm::CodeGenFileType::AssemblyFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        exit(1);
    }

    pass.run(*TheModule);
    dest.flush();
}

/* ---------------------------------------------------------------------
   ----------------------- LLVM Compile and Dump -----------------------
   --------------------------------------------------------------------- */

void AST::llvm_compile_and_dump()
{
    TheModule = std::make_unique<llvm::Module>("grace program", TheContext);

    // Add optimization functions
    FPM_Optimizations();

    // Add Library Functions
    llvmAddLibrary();

    // Emit the program code.
    llvm::Value *main_function = compile();
    llvm::Function *main = MainCodeGen(main_function);
    
    // Verify the IR.
    bool bad = verifyModule(*TheModule, &llvm::errs());
    if (bad)
    {
        std::cerr << "The IR is bad!" << std::endl;
        TheModule->print(llvm::errs(), nullptr);
        std::exit(1);
    }

    // // Print out the IR.
    // TheModule->print(llvm::outs(), nullptr);
    // std::cout << std::endl;

    /* Dump IR and final code to correct files/stdout */
    if (genIntermediate)
        /* Dump intermidiate code to stdout */
        emitLLVMIR("-");
    else if (genFinal)
        /* Dump final code to stdout */
        emitAssembly("-");
    else
    {
        /* Dump intermidiate code to *.imm file */
        std::string imm_filename = filename.substr(0, filename.find_last_of('.')) + ".imm";
        emitLLVMIR(imm_filename);
        /* Dump final code to *.asm file */
        std::string asm_filename = filename.substr(0, filename.find_last_of('.')) + ".asm";
        emitAssembly(asm_filename);
    }
    if (optimize)
        TheFPM->run(*main);
}

/* ---------------------------------------------------------------------
   ----------------------------- LogErrorV -----------------------------
   --------------------------------------------------------------------- */

llvm::Value *AST::LogErrorV(const char *Str) const
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

/* ---------------------------------------------------------------------
   ---------------------- CreateEntryBlockAlloca -----------------------
   --------------------------------------------------------------------- */

llvm::AllocaInst *AST::CreateEntryBlockAlloca(llvm::Function *TheFunction,
                                              llvm::StringRef VarName,
                                              llvmType *llvm_type)
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                           TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm_type, nullptr, VarName);
}

/* ---------------------------------------------------------------------
   ------------------------------- UTILS -------------------------------
   --------------------------------------------------------------------- */

std::string getMangledName(const char * name, int scope)
{
    if (scope > 0)
        return std::string(name) + '_' + std::to_string(scope);
    else
        return std::string(name);
}
std::string getFunctionStackFrameStructName(std::string func_name)
{
    return "stack_frame_struct_" + func_name;
}

std::string getStackFrameName(std::string func_name)
{
    return "stack_frame_" + func_name;
}

/**
 * This function retrieves the address of a stack frame based on the declaration and usage depths.
 * It navigates through nested stack frames if necessary and returns the final stack frame address.
 *
 * Parameters:
 * @param decl_depth: The depth at which the variable (orfunction) was declared.
 * @param usage_depth: The depth at which the variable (or function) is used.
 * @param final_stack_frame_type: A pointer to store the type of the final stack frame (optional).
 *
 * @returns: An llvm::Value pointer representing the address of the stack frame.
 */
llvm::Value *AST::getStackFrameAddr(unsigned int decl_depth, unsigned int usage_depth, llvmType **final_stack_frame_type)
{
    // Retrieve the current function (caller).
    llvm::Function *Caller = Builder.GetInsertBlock()->getParent();
    std::string func_name = Caller->getName().str();

    // Get the address and type of the current stack frame.
    llvm::Value *stack_frame_addr = NamedValues[getStackFrameName(func_name)];
    llvmType *stack_frame_type = llvm::StructType::getTypeByName(TheContext, getFunctionStackFrameStructName(func_name));

    // Calculate the difference between usage depth and declaration depth.
    unsigned int depth_difference = usage_depth - decl_depth;
    while (depth_difference-- > 0)
    {
        // Move to the next outer function.
        func_name = OuterFunction[func_name];

        // Get the address and type of the next stack frame.
        llvmType *next_stack_frame_type = llvm::StructType::getTypeByName(TheContext, getFunctionStackFrameStructName(func_name));
        stack_frame_addr = Builder.CreateLoad(next_stack_frame_type->getPointerTo(), Builder.CreateStructGEP(stack_frame_type, stack_frame_addr, 0));
        stack_frame_type = next_stack_frame_type;
    }
    // If provided, store the type of the final stack frame in final_stack_frame_type.
    if (final_stack_frame_type)
        *final_stack_frame_type = stack_frame_type;

    // Return the address of the final stack frame.
    return stack_frame_addr;
}

bool isTopLevel(std::string func_name)
{
    return AST::FunctionDepth[func_name] <= 2;
}

llvmType *getLLVMType(Type t, llvm::LLVMContext& context)
{
    if (equalType(t, typeVoid))
        return llvmType::getVoidTy(context);
    else if (equalType(t, typeInteger))
        return llvmType::getInt64Ty(context);
    else if (equalType(t, typeChar))
        return llvmType::getInt8Ty(context);
    else
    {
        llvmType *elementType = getLLVMType(t->refType, context);

        // Array
        if (t->size > 0)
            return llvm::ArrayType::get(elementType, t->size);
        else
            return elementType;
    }
}

// Flags and filename
bool optimize, genFinal, genIntermediate;
std::string filename;