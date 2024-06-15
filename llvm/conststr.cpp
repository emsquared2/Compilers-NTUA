#include "conststr.hpp"

ConstStr::ConstStr(std::string s) : name(s) { dereference = true; }
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
    RepInteger len = name.length() + 1;
    type = typeArray(len, typeChar);
    // type = typeIArray(typeChar);
    ref = true;
}
llvm::Value* ConstStr::compile_ptr()
{
    return Builder.CreateGlobalStringPtr(llvm::StringRef(name));
}

llvm::Value * ConstStr::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{
    // Insert zero index at the beginning of offsets
    // This is necessary to dereference the array base address and access the array itself
    if (dereference)
        offsets->insert(offsets->begin(), c64(0));
    *t = getLLVMType(type, TheContext);
    return compile_ptr();
}

llvm::Value* ConstStr::compile()
{
    llvm::Value *VarAddr = compile_ptr();
    if (!VarAddr)
    {
        std::string msg = "Error while compiling ConstStr: " + name + ".";
        return LogErrorV(msg.c_str());
    }

    /* Load the value */
    return Builder.CreateLoad(getLLVMType(type, TheContext), VarAddr);
}