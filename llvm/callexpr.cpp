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
    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);

    if (function->entryType != ENTRY_FUNCTION)
        SemanticError("Could not find function name.");

    // Check if function is called with correct number and type of arguments
    SymbolEntry *argument = function->u.eFunction.firstArgument;

    std::vector<Expr *> e_list;

    if (expr_list != nullptr)
        e_list = expr_list->getExprList();

    int counter = 0;

    for (auto e = e_list.rbegin(); e != e_list.rend(); ++e)
    {
        // More parameters than expected
        if (argument == NULL)
        {
            std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
            SemanticError(msg.c_str());
        }

        (*e)->type_check(argument->u.eParameter.type);

        /* Check if Expr e is a LValue */
        LValue *lvalue_ptr = dynamic_cast<LValue *>((*e));
        if (argument->u.eParameter.mode == PASS_BY_REFERENCE && !lvalue_ptr)
            SemanticError("Parameter defined as pass-by-reference must be an lvalue.");

        argument = argument->u.eParameter.next;

        counter++;
    }

    // Fewer parameters than expected
    if (argument != NULL)
    {
        // Find true number of arguments
        while (argument != NULL)
        {
            argument = argument->u.eParameter.next;
            counter++;
        }

        std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
        SemanticError(msg.c_str());
    }
    type = function->u.eFunction.resultType;
}

llvm::Value *CallExpr::compile() const
{

    llvm::StringRef Callee = id->getName();
    std::vector<Expr *> Args = (expr_list) ? expr_list->getExprList() : std::vector<Expr *> {};

    // Look up the name in the global module table.
    llvm::Function *CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value *> ArgsV;
    for (int i = Args.size() - 1; i >= 0; --i)
    {
        ArgsV.push_back(Args[i]->compile());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}
