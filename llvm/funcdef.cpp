#include "funcdef.hpp"

FuncDef::FuncDef(Header *h, LocalDefList *ldl, Block *b) : header(h), local_def_list(ldl), block(b) {}
FuncDef::~FuncDef()
{
    delete header;
    delete local_def_list;
    delete block;
}
void FuncDef::printOn(std::ostream &out) const
{
    out << "FuncDef(";
    out << *header << " " << *local_def_list << " " << *block;
    out << ")";
}
void FuncDef::sem()
{
    header->sem();
    local_def_list->sem();
    block->sem();

    SymbolEntry *function = lookupEntry(header->getId()->getName(), LOOKUP_ALL_SCOPES, true);

    if (!equalType(header->getReturnType(), typeVoid) && !returnedFunction.back())
    {
        std::string func_name = header->getId()->getName();
        std::string msg = "Non-void function " + func_name + " has no return statement.";
        SemanticError(msg.c_str());
    }

    returnedFunction.pop_back();
    closeScope();
}

void FuncDef::ProgramSem()
{
    /* Program should:
     *      1) NOT take parameters
     *     2) Return nothing, i.e. have type typeVoid
     */

    if (header->getFParamList() != nullptr)
        SemanticError("Program cannot take parameters.");

    if (!equalType(header->getReturnType(), typeVoid))
        SemanticError("Program should have type void.");

    sem();
}

llvm::Function *FuncDef::compile() const
{

    llvm::BasicBlock *BB_ofAbovelvelFunc = Builder.GetInsertBlock();

    llvm::Function *function = header->compile();
    if (!function)
        return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", function);
    Builder.SetInsertPoint(BB);

    unsigned int current_arg = 0;
    std::vector<std::string> llvm_param_names = header->getLLVM_param_names();
    std::vector<llvmType *> llvm_param_types = header->getLLVM_param_types();

    for (auto &Arg : function->args())
    {
        // Create an alloca for this variable.
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(function, llvm::StringRef(llvm_param_names[current_arg]), llvm_param_types[current_arg]);

        // Store the initial value into the alloca.
        Builder.CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[std::string(llvm_param_names[current_arg++])] = Alloca;
    }

    std::vector<LocalDef *> locals = local_def_list->getLocals();

    for (auto l = locals.rbegin(); l != locals.rend(); ++l)
    {
        (*l)->compile();
        // test it outside of the loop
        Builder.SetInsertPoint(BB);
    }

    block->compile();

    if (!Builder.GetInsertBlock()->getTerminator())
    {
        if (equalType(header->getReturnType(), typeInteger) || equalType(header->getReturnType(), typeChar))
            Builder.CreateRet(c64(0));
        else
            Builder.CreateRetVoid();
    }

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*function);

    Builder.SetInsertPoint(BB_ofAbovelvelFunc);

    return function;
}