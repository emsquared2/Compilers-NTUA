#include "callstmt.hpp"


CallStmt::CallStmt(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
CallStmt::~CallStmt()
{
    delete id;
    delete expr_list;
}
void CallStmt::printOn(std::ostream &out) const
{
    out << "CallStmt(" << *id << "(";
    if (expr_list != nullptr)
        out << *expr_list;
    out << "))";
}

void CallStmt::sem()
{
    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);

    if (function->entryType != ENTRY_FUNCTION)
        SemanticError("Could not find function name.");

    // Check if function is called with correct number and type of arguments
    SymbolEntry *argument = function->u.eFunction.firstArgument;

    std::vector<Expr *> e_list = (expr_list != nullptr) ? expr_list->getExprList() : std::vector<Expr *>{};

    int counter = 0;

    for (Expr *e : e_list)
    {
        // More parameters than expected
        if (argument == NULL)
        {
            std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
            SemanticError(msg.c_str());
        }

        e->type_check(argument->u.eParameter.type);

        /* Check if Expr e is a LValue */
        LValue * lvalue_ptr = dynamic_cast<LValue *>(e);
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

    /* I think this is the only difference:
     * CallStmt is not an Expr so it does not have a type.
     * Function statements should have void return type.
     */
    if (!equalType(function->u.eFunction.resultType, typeVoid))
        SemanticError("CallStmt: Function should have void return type");
}