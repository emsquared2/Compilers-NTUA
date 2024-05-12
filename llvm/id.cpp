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

llvm::Value *Id::compile_ptr() const
{
    // Look this variable up in the function.
    llvm::AllocaInst *A = NamedValues[name + '_' + std::to_string(scope) + '_'];
    if (!A)
    {
        std::string msg = "Id: Unknown variable name: " + name + '_' + std::to_string(scope) + '_' + ".";
        return LogErrorV(msg.c_str());
    }

    std::string mangled_name = name + '_' + std::to_string(scope) + '_';
    // // Load the value.
    return Builder.CreateLoad(llvm::PointerType::get(A->getAllocatedType(), 0), A, mangled_name.c_str());

    // return A;
}

llvm::Value *Id::compile() const
{
    // Look this variable up in the function.
    llvm::AllocaInst *A = NamedValues[name + '_' + std::to_string(scope) + '_'];
    if (!A)
    {
        std::string msg = "Id: Unknown variable name: " + name + '_' + std::to_string(scope) + ".";
        return LogErrorV(msg.c_str());
    }

    std::string mangled_name = name + '_' + std::to_string(scope) + '_';
    // // Load the value.
    return Builder.CreateLoad(A->getAllocatedType(), A, mangled_name.c_str());

    // return A;
}
