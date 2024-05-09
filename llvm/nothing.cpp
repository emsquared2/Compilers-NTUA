#include "nothing.hpp"

Nothing::Nothing() {}
Nothing::~Nothing() {}
void Nothing::printOn(std::ostream &out) const
{
    out << "Nothing";
}
void Nothing::sem()
{
    type = typeVoid;
}
llvm::Value* Nothing::compile() const
{
    return nullptr;
}