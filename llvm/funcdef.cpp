#include "funcdef.hpp"

FuncDef::FuncDef(Header *h, LocalDefList *ldl, Block *b) : header(h), local_def_list(ldl), block(b) {}
FuncDef::~FuncDef()
{
    delete header;
    delete local_def_list;
    delete block;
}
void FuncDef::printOn(std::ostream &out) const
{
    out << "FuncDef(";
    out << *header << " " << *local_def_list << " " << *block;
    out << ")";
}
void FuncDef::sem()
{
    header->sem();

    mangled_name = header->getHMangledName();

    if (local_def_list) {
        for(LocalDef *local : local_def_list->getLocals()) {
            local->sem();
            local->setOuterFunction(mangled_name);
        }
    }

    block->sem();

    lookupEntry(header->getId()->getName(), LOOKUP_ALL_SCOPES, true);

    if (!equalType(header->getReturnType(), typeVoid) && !returnedFunction.back())
    {
        std::string func_name = header->getId()->getName();
        std::string msg = "Non-void function " + func_name + " has no return statement.";
        SemanticError(msg.c_str());
    }

    returnedFunction.pop_back();
    closeScope();
}

void FuncDef::ProgramSem()
{
    /* Program should:
     *      1) NOT take parameters
     *      2) Return nothing, i.e. have type typeVoid
     */

    if (header->getFParamList() != nullptr)
        SemanticError("Program cannot take parameters.");

    if (!equalType(header->getReturnType(), typeVoid))
        SemanticError("Program should have type void.");

    sem();
}

llvm::Function *FuncDef::compile()
{
    llvm::BasicBlock *BB_ofAbovelvelFunc = Builder.GetInsertBlock();

    llvm::Function *function = header->compile();
    if (!function)
        return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", function);
    Builder.SetInsertPoint(BB);

    unsigned int current_arg = 0;
    std::vector<std::string> llvm_param_names = header->getLLVM_param_names();
    std::vector<llvmType *> llvm_param_types = header->getLLVM_param_types();

    for (auto &Arg : function->args())
    {
        // Create an alloca for this variable.
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(function, llvm::StringRef(llvm_param_names[current_arg]), llvm_param_types[current_arg]);

        // Store the initial value into the alloca.
        Builder.CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[llvm_param_names[current_arg++]] = Alloca;
    }

    llvmType *stack_frame_type = createFunctionStructType();

    for (LocalDef *l : local_def_list->getLocals())
    {
        l->compile();
        Builder.SetInsertPoint(BB);
    }

    createFunctionStackFrame(stack_frame_type);
    populateFunctionStackFrame();

    block->compile();

    if (!Builder.GetInsertBlock()->getTerminator())
    {
        if (equalType(header->getReturnType(), typeInteger) || equalType(header->getReturnType(), typeChar))
            Builder.CreateRet(c64(0));
        else
            Builder.CreateRetVoid();
    }

    if (optimize)
    {
        optimizeFunc(function);
    }

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*function);

    Builder.SetInsertPoint(BB_ofAbovelvelFunc);

    return function;
}

void FuncDef::setOuterFunction(std::string outer_func_name)
{
    OuterFunction[mangled_name] = outer_func_name;
}

/*
 * This function builds the LLVM structure type for a function's stack frame.
 * It includes captured variables, parameters, and a static link to the outer 
 * function's stack frame (if the function is nested, i.e. non-top-level). 
 */
llvmType * FuncDef::createFunctionStructType()
{
    if (!isTopLevel(mangled_name))
    {
        // Add a static link to the function signature to access the outer function's stack frame
        header->addStaticLinkToFunctionSignature(&captured_names, &captured_types);
        // Mark the static link as a reference
        captured_ref.push_back(true);
        // Record the offset of the static link in the stack frame
        CapturedVariableOffset["static_link_" + mangled_name] = 0;
    }

    // Add captured parameters to the function's stack frame
    header->addCapturedParameters(&captured_names, &captured_types, &captured_ref);

    // Add captured variables
    if(local_def_list) 
    {
        for(LocalDef *l : local_def_list->getLocals())
        {
            Decl *decl = dynamic_cast<Decl*>(l);
            if (decl)
                decl->addCapturedParameters(&captured_names, &captured_types, &captured_ref);
        }
    }

    // Generate the name for the function's stack frame structure
    std::string struct_name = getFunctionStackFrameStructName(mangled_name);
    // Create and return the LLVM structure type for the function's stack frame
    return llvm::StructType::create(TheContext, captured_types, struct_name);
}

/*
 * This function creates an allocation for the stack frame of a function.
 * This stack frame holds all local variables, parameters, and any captured
 * variables that need to be accessible within the function. 
 */
void FuncDef::createFunctionStackFrame(llvmType *stack_frame_type)
{
    std::string stack_frame_name = getStackFrameName(mangled_name);
    // Allocate memory on the stack for the stack frame
    llvm::AllocaInst *Alloca = Builder.CreateAlloca(stack_frame_type, nullptr, stack_frame_name);
    // Store the allocation in the NamedValues map
    NamedValues[stack_frame_name] = Alloca;
}

void FuncDef::populateFunctionStackFrame()
{
    // Retrieve the address and type of the function's stack frame
    llvm::Value *stackFrameAddress = NamedValues[getStackFrameName(mangled_name)];
    llvmType *stackFrameType = llvm::StructType::getTypeByName(TheContext, getFunctionStackFrameStructName(mangled_name));

    for (int idx = 0; idx < captured_names.size(); idx++)
    {
        // Calculate the address within the stack frame where the captured variable will be stored
        llvm::Value *stackFrameCapturedVarAddress = Builder.CreateStructGEP(stackFrameType, stackFrameAddress, idx);
        
        // Get the address where captured variable is stored
        llvm::Value *capturedVariableAddress;
        if (captured_ref[idx])
            capturedVariableAddress = Builder.CreateLoad(captured_types[idx], NamedValues[captured_names[idx]]);
        else
            capturedVariableAddress = NamedValues[captured_names[idx]];

        // Store the address of the captured variable into the stack frame
        Builder.CreateStore(capturedVariableAddress, stackFrameCapturedVarAddress);
    }
}

void FuncDef::optimizeFunc(llvm::Function *function)
{
    TheFPM->run(*function);
}