#include "constchar.hpp"

ConstChar::ConstChar(char v) : var(v) {}
ConstChar::~ConstChar() {}
void ConstChar::printOn(std::ostream &out) const
{
    out << "ConstChar(" << var << ")";
}
void ConstChar::sem()
{
    std::cout << "ConstChar Sem..." << std::endl;
    type = typeChar;
}