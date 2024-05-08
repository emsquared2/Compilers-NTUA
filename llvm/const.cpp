#include "const.hpp"

Const::Const(int v) : val(v) {}
Const::~Const() {}
void Const::printOn(std::ostream &out) const
{
    out << "Const(" << val << ")";
}
int Const::getVal() 
{
    return val;
}
void Const::sem()
{
    type = typeInteger;
}

llvm::Value* Const::compile() const
{
    return c64(val);
}