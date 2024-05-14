#include "constchar.hpp"

ConstChar::ConstChar(char v) : var(v) {}
ConstChar::~ConstChar() {}
void ConstChar::printOn(std::ostream &out) const
{
    out << "ConstChar(" << var << ")";
}
void ConstChar::sem()
{
    type = typeChar;
}

llvm::Value* ConstChar::compile()
{
    return c8(var);
}