#include "conststr.hpp"

ConstStr::ConstStr(std::string s) : name(s) {}
void ConstStr::printOn(std::ostream &out) const
{
    out << "ConstStr(" << name << ")";
}
const char *ConstStr::getName() const
{
    return name.c_str();
}
void ConstStr::sem()
{
    // RepInteger len = str.length() + 1;
    // type = typeArray(len, typeChar);
    type = typeIArray(typeChar);
    ref = true;
}
llvm::Value* ConstStr::compile_ptr()
{
    return Builder.CreateGlobalStringPtr(llvm::StringRef(name));
}

llvm::Value * ConstStr::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{
    *t = getLLVMType(type, TheContext);
    return compile_ptr();
}