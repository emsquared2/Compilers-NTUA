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
    std::cout << "Assign Sem..." << std::endl;

    l_value->sem();
    expr->sem();

    // this probably will existin in l_value sem so it will not be needed here
    // lookupEntry(l_value->getName(), LOOKUP_ALL_SCOPES, true);

    Type l_value_type = l_value->getType();
    Type expr_type = expr->getType();

    // Check that types match
    if (!equalType(l_value_type, expr_type))
        SemanticError("Assign: L_value and Expr type mismatch.");

    // Check that their type is typeInteger or typeChar
    if (!equalType(l_value_type, typeInteger) && !equalType(l_value_type, typeChar))
        SemanticError("Assign: L_value's type should be typeInteger or typeChar");

    if (!equalType(expr_type, typeInteger) && !equalType(expr_type, typeChar))
        SemanticError("Assign: Expr's type should be typeInteger or typeChar");
}
