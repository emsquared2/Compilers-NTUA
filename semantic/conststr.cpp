#include "conststr.hpp"

ConstStr::ConstStr(std::string s) : str(s) {}
void ConstStr::printOn(std::ostream &out) const
{
    out << "ConstStr(" << str << ")";
}
const char *ConstStr::getName() const
{
    return str.c_str();
}
void ConstStr::sem()
{
    RepInteger len = str.length() + 1;
    type = typeArray(len, typeChar);
    // type = typeIArray(typeChar);
}