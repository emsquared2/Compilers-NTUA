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
    std::cout << "Const Sem..." << std::endl;
    type = typeInteger;
}
