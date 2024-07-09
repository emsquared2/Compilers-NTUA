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
    // Get the return type of the function
    Type return_type = returnType.back();

    // Check if the function's return type is void
    if (equalType(return_type, typeVoid))
    {
        // If the return type is void, ensure no expression is returned
        if (expr != nullptr)
        {
            SemanticError("Return type should be void");
        }
    }
    // Check if the function's return type is integer or character
    else if (equalType(return_type, typeInteger) || equalType(return_type, typeChar))
    {
        // If the return type is integer or character, ensure the expression type matches the return type
        if (expr != nullptr)
            expr->typeCheck(return_type);
        else
            SemanticError("Return's expression type does not match return type.");
    }
    else
    {
        SemanticError("Return type should be int, char or void");
    }
    // Indicate that the function has a return statement
    returnedFunction.back() = true;
}

llvm::Value * Return::compile()
{
    // If there is an expression, compile it and create a return instruction with the expression value
    if (expr != nullptr)
        Builder.CreateRet(expr->compile());
    else
        // If there is no expression, create a void return instruction
        Builder.CreateRetVoid();

    return nullptr;
}