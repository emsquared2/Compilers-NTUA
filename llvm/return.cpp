#include "return.hpp"

Return::Return(Expr *e) : expr(e) {}

void Return::printOn(std::ostream &out) const 
{
    out << "Return(";
    if (expr != nullptr)
        out << *expr;
    out << ")";
}

void Return::sem()
{
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

llvm::Value * Return::compile()
{
    if (expr != nullptr)
        Builder.CreateRet(expr->compile());
    else
        Builder.CreateRetVoid();

    return nullptr;
}