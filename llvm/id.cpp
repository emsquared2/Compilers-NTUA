#include "id.hpp"

Id::Id(std::string s) : name(s) {}
void Id::printOn(std::ostream &out) const
{
    out << "Id(" << name << ")";
}
const char *Id::getName() const
{
    return name.c_str();
}
std::string Id::getMangledName() const
{
    return name + '_' + std::to_string(scope) + '_';
}

void Id::sem()
{
    // Check if the identifier exists.
    SymbolEntry *e = lookupEntry(this->getName(), LOOKUP_ALL_SCOPES, true);

    setScope(e->scopeId);

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
        // if (e->u.eParameter.mode == PASS_BY_REFERENCE)
        //     ref = true;
        break;
    // Can it be Temporary?
    case ENTRY_TEMPORARY:
        type = e->u.eTemporary.type;
        break;
    default:
        SemanticError("Invalid entry for identifier. This should never be reached");
        break;
    }
}

llvm::Value * Id::compile_ptr()
{
    std::string mangled_name = getMangledName();

    // Look this variable up in the function.
    llvm::Value * LValAddr = NamedValues[mangled_name];
    if (!LValAddr) {
        std::string msg = "Id: Unknown variable name: " + mangled_name + ".";
        return LogErrorV(msg.c_str());
    }
    if (!ref) 
        return LValAddr;
    else
        return Builder.CreateLoad(llvm::PointerType::get(getLLVMType(type, TheContext), 0), LValAddr); 
}

llvm::Value * Id::compile()
{
    llvm::Value *LValAddr = compile_ptr();
    if (!LValAddr) 
    {
        std::string msg = "Id: Unknown variable name: " + getMangledName() + ".";
        return LogErrorV(msg.c_str());
    }
    // Load the value
    return Builder.CreateLoad(getLLVMType(type, TheContext), LValAddr);
}

llvm::Value * Id::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{

    /* push 0 in the beginning of the offsets to dereference the GEP pointer */
    offsets->insert(offsets->begin(), c64(0));

    /* end of the recursion - return the address and the type of the matrix */
    *t = getLLVMType(type, TheContext);
    return compile_ptr();
}
