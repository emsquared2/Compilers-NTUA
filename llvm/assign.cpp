#include "assign.hpp"

Assign::Assign(LValue *l, Expr *e) : l_value(l), expr(e) {}

Assign::~Assign()
{
    delete l_value;
    delete expr;
}

void Assign::printOn(std::ostream &out) const
{
    out << "Assign(" << *l_value << " <- " << *expr << ")";
}

void Assign::sem()
{
    l_value->sem();
    expr->sem();

    Type l_value_type = l_value->getType();
    Type expr_type = expr->getType();

    // Ensure that the types of the LValue and the expression are the same.
    if (!equalType(l_value_type, expr_type))
        SemanticError("Assign: L_value and Expr type mismatch.");

    // Check that the type of the LValue is either typeInteger or typeChar.
    if (!equalType(l_value_type, typeInteger) && !equalType(l_value_type, typeChar))
        SemanticError("Assign: L_value's type should be typeInteger or typeChar");

    // Check that the type of the expression is either typeInteger or typeChar.
    if (!equalType(expr_type, typeInteger) && !equalType(expr_type, typeChar))
        SemanticError("Assign: Expr's type should be typeInteger or typeChar");
}

llvm::Value * Assign::compile()
{
    llvm::Value *LValAddr = l_value->compile_ptr();
    if (!LValAddr)
        return LogErrorV("Assign: LValue could not be compiled.");

    llvm::Value * ExprValue = expr->compile();
    if (!ExprValue)
        return LogErrorV("Assign: Expression could not be compiled.");

    Builder.CreateStore(ExprValue, LValAddr);
    return nullptr;
}