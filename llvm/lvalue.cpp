#include "lvalue.hpp"

const char * LValue::getName() const
{
    const char* name = "LValue dummy name";
    return name;
}

void LValue::setScope(int sc) 
{
    scope = sc;
}

int LValue::getScope()
{
    return scope;
}

void LValue::setRef(bool r)
{
    ref = r;
}

bool LValue::getRef()
{
    return ref;
}

// llvm::Value * LValue::compile() { return nullptr; }
// llvm::Value * LValue::compile_ptr() { return nullptr; }
// llvm::Value * LValue::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t) { return nullptr; }

llvm::Value * LValue::compile_ptr()
{
    std::string mangled_name = name + '_' + std::to_string(scope);
    llvm::Value * VarAddr = NamedValues[mangled_name];
    
    if (!VarAddr)
        return LogErrorV("LValue Compile_ptr: Execution shoudln't reach this point.");

    return (ref) ? Builder.CreateLoad(llvm::PointerType::get(getLLVMType(type, TheContext), 0), VarAddr) : VarAddr;
}

llvm::Value* LValue::compile()
{
    llvm::Value * VarAddr = compile_ptr();
    if (!VarAddr)
        return LogErrorV("LValue Compile: Execution shouldn't reach this point.");

    /* Load the value */
    return Builder.CreateLoad(getLLVMType(type, TheContext), VarAddr);
}