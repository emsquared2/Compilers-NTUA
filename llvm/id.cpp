#include "id.hpp"

Id::Id(std::string s) : name(s) { dereference = false; }

void Id::printOn(std::ostream &out) const
{
    out << "Id(" << name << ")";
}
const char *Id::getName() const
{
    return name.c_str();
}

void Id::sem()
{
    // Check if the identifier exists.
    SymbolEntry *e = lookupEntry(this->getName(), LOOKUP_ALL_SCOPES, true);

    switch (e->entryType)
    {
    case ENTRY_VARIABLE:
        type = e->u.eVariable.type;
        break;
    case ENTRY_CONSTANT:
        SemanticError("Id cannot be a constant.");
        break;
    case ENTRY_FUNCTION:
        SemanticError("Id cannot be a function.");
        break;
    case ENTRY_PARAMETER:
        type = e->u.eParameter.type;
        if (e->u.eParameter.mode == PASS_BY_REFERENCE)
            ref = true;  
        break;
    // Can it be Temporary?
    case ENTRY_TEMPORARY:
        type = e->u.eTemporary.type;
        break;
    default:
        SemanticError("Invalid entry for identifier. This should never be reached");
        break;
    }
    dereference = e->u.eParameter.type->size != UNKNOWN_SIZE;
    mangled_name = getMangledName(name.c_str(), e->scopeId);

    decl_depth = e->nestingLevel;
    usage_depth = currentScope->nestingLevel;
    
    if (decl_depth < usage_depth)
    {
        is_captured = true;
        ref = true;
        CapturedVariables.insert(mangled_name);
    }
}

llvm::Value * Id::compile_ptr()
{
    // Look this variable up in the function.
    llvm::Value *LValAddr = is_captured ? getCapturedVarAddr() : NamedValues[mangled_name];
    if (!LValAddr) {
        std::string msg = "Id: Unknown variable name: " + mangled_name + ".";
        return LogErrorV(msg.c_str());
    }
    return ref ? Builder.CreateLoad(llvm::PointerType::get(getLLVMType(type, TheContext), 0), LValAddr) : LValAddr;
}

llvm::Value * Id::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{
    // Insert zero index at the beginning of offsets
    // This is necessary to dereference the array base address and access the array itself
    if (dereference)
        offsets->insert(offsets->begin(), c64(0));
    
    *t = getLLVMType(type, TheContext);
    return compile_ptr();
}

llvm::Value * Id::compile()
{
    llvm::Value *LValAddr = compile_ptr();
    if (!LValAddr) 
    {
        std::string msg = "Id: Unknown variable name: " + mangled_name + ".";
        return LogErrorV(msg.c_str());
    }
    return Builder.CreateLoad(getLLVMType(type, TheContext), LValAddr);
}

llvm::Value * Id::getCapturedVarAddr()
{
    // Type of the stack frame
    llvmType *stack_frame_type;
    // Get the address of the stack frame
    llvm::Value *stack_frame_addr = getStackFrameAddr(decl_depth, usage_depth, &stack_frame_type);
    // Get the index of the captured variable in the stack frame
    unsigned int index = CapturedVariableOffset[mangled_name];

    // Create a GEP (GetElementPtr) instruction to access the captured variable's address within the stack frame.
    return Builder.CreateStructGEP(stack_frame_type, stack_frame_addr, index);
}