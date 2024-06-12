#include "id.hpp"

Id::Id(std::string s) : name(s)
{
    // if (name == "main")
    //     name = "grc_main";
}
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
    llvm::Value *LValAddr = is_captured ? CapturedVarAddr() : NamedValues[mangled_name];
    if (!LValAddr) {
        std::string msg = "Id: Unknown variable name: " + mangled_name + ".";
        return LogErrorV(msg.c_str());
    }
    return ref ? Builder.CreateLoad(llvm::PointerType::get(getLLVMType(type, TheContext), 0), LValAddr) : LValAddr;
}

llvm::Value * Id::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{
    /* push 0 in the beginning of the offsets to dereference the GEP pointer */
    // offsets->insert(offsets->begin(), c64(0));
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

// TODO
llvm::Value * Id::CapturedVarAddr()
{
    return NamedValues[mangled_name];
}