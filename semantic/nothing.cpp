#include "nothing.hpp"

Nothing::Nothing() {}
Nothing::~Nothing() {}
void Nothing::printOn(std::ostream &out) const
{
    out << "Nothing";
}
void Nothing::sem()
{
    std::cout << "Nothing Sem..." << std::endl;
    type = typeVoid;
}