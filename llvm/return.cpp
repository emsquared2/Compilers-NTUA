#include "return.hpp"

Return::Return(Expr *e) : expr(e) {}

void Return::printOn(std::ostream &out) const 
{
    out << "Return(";
    if (expr != nullptr)
        out << *expr;
    out << ")";
}

int Return::ReturnValue() 
{
    // return (expr != nullptr) ? expr->eval() : 0;
}

void Return::sem()
{
    /* TODO: Get return type.
     * There are 2 options:
     *      a) Use a stack that keeps the return types (function decls are nested so you only need to check the last element).
     *      b) Implement a method in symbol.c in order to get the return type from the last function in scope.
     */

    SymbolEntry *func = lookupLastFunction();

    Type return_type = func->u.eFunction.resultType;

    // If return type is void, then expr should be nullptr
    if (equalType(return_type, typeVoid))
    {
        if (expr != nullptr)
        {
            SemanticError("Return type should be void");
        }
    }
    else if (equalType(return_type, typeInteger) || equalType(return_type, typeChar))
    {
        if (expr != nullptr)
            expr->type_check(return_type);
        else
            SemanticError("Return's expression type does not match return type.");
    }
    else
    {
        SemanticError("Return type should be int, char or void");
    }

    // Declare that function has return statement
    returnedFunction.back() = true;
}