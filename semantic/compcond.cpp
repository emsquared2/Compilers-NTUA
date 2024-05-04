#include "compcond.hpp"

CompCond::CompCond(Expr *l, char *s, Expr *r) : left(l), op(s), right(r) {}

CompCond::~CompCond()
{
    delete left;
    delete right;
}

void CompCond::printOn(std::ostream &out) const
{
    out << op << "(" << *left << ", " << *right << ")";
}

void CompCond::sem()
{
    std::cout << "CopmCond Sem..." << std::endl;

    left->sem();
    right->sem();

    Type left_type = left->getType();
    Type right_type = right->getType();

    // Check operands' type (typeInteher or typeChar)
    if (!equalType(left_type, typeInteger) && !equalType(left_type, typeChar))
        SemanticError("CompCond: Left operand should be typeInteger or typeChar");

    if (!equalType(right_type, typeInteger) && !equalType(right_type, typeChar))
        SemanticError("CompCond: Right operand should be typeInteger or typeChar");

    // Check if right and left operand have the same type
    if (!equalType(left_type, right_type))
        SemanticError("CompCond: Operands should have the same type.");

    type = typeBoolean;
}