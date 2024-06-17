#include "callexpr.hpp"

CallExpr::CallExpr(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
CallExpr::~CallExpr()
{
    delete id;
    delete expr_list;
}
void CallExpr::printOn(std::ostream &out) const
{
    out << "CallExpr(" << *id << "(";
    if (expr_list != nullptr)
        out << *expr_list;
    out << "))";
}
void CallExpr::sem()
{
    // Look up the function in the symbol table
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);

    mangled_name = getMangledName(id->getName(), function->scopeId);

    // Get the callee and caller depths for scope handling
    callee_depth = FunctionDepth[mangled_name];
    caller_depth = currentScope->nestingLevel;

    // Check if the looked-up entry is indeed a function
    if (function->entryType != ENTRY_FUNCTION)
        SemanticError("Could not find function name.");

    // Check the function's parameters against the provided arguments
    SymbolEntry *argument = function->u.eFunction.firstArgument;
    std::vector<Expr *> e_list = (expr_list != nullptr) ? expr_list->getExprList() : std::vector<Expr *>{};

    int counter = 0;

    for (Expr *e : e_list)
    {
        // More parameters than expected
        if (!argument)
        {
            std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
            SemanticError(msg.c_str());
        }
        // Type check the argument
        e->typeCheck(argument->u.eParameter.type);

        // Check if the argument is a LValue for pass-by-reference
        LValue *lvalue_ptr = dynamic_cast<LValue *>(e);
        if (argument->u.eParameter.mode == PASS_BY_REFERENCE && !lvalue_ptr)
            SemanticError("Parameter defined as pass by reference must be an lvalue.");

        ref.push_back(argument->u.eParameter.mode == PASS_BY_REFERENCE);

        // Move to the next parameter
        argument = argument->u.eParameter.next;
        counter++;
    }

    // Fewer parameters than expected
    if (argument != NULL)
    {
        // Calculate the true number of arguments
        while (argument != NULL)
        {
            argument = argument->u.eParameter.next;
            counter++;
        }

        std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
        SemanticError(msg.c_str());
    }
    // Set the return type of the function call
    type = function->u.eFunction.resultType;
}

llvm::Value *CallExpr::compile()
{
    // Look up the function in the LLVM module
    llvm::Function *CalleeF = TheModule->getFunction(mangled_name);
    if (!CalleeF) {
        std::string msg = "CallExpr: Unknown function referenced --> " + mangled_name;
        return LogErrorV(msg.c_str());
    }

    // Get the list of arguments for the function call
    std::vector<Expr *> Args = (expr_list) ? expr_list->getExprList() : std::vector<Expr *> {};
    std::vector<llvm::Value *> ArgsV;

    // If the function is not top-level, add a static link for accessing the outer function's stack frame
    if (!isTopLevel(mangled_name))
    {
        ref.push_back(true);
        llvm::Value *stackFrameAddr = getStackFrameAddr(callee_depth, caller_depth);
        ArgsV.push_back(stackFrameAddr);
    }

    // Compile each argument
    for (int i = 0; i < Args.size(); ++i)
    {
        llvm::Value *ExprV_A = ref[i] ? Args[i]->compile_ptr() : Args[i]->compile();
        ArgsV.push_back(ExprV_A);
        if (!ArgsV.back())
            return nullptr;
    }
    // Create the function call instruction
    return Builder.CreateCall(CalleeF, ArgsV, "callexprtmp");
}
