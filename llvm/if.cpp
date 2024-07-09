#include "if.hpp"

If::If(Cond *c, Stmt *s1, Stmt *s2 = nullptr) : cond(c), stmt1(s1), stmt2(s2) {}

If::~If()
{
    delete cond;
    delete stmt1;
    delete stmt2;
}

void If::printOn(std::ostream &out) const
{
    out << "If(" << *cond;
    if (stmt1 != nullptr) 
        out << ", " << *stmt1;
    if (stmt2 != nullptr)
        out << " Else " << *stmt2;
    out << ")";
}

void If::sem()
{
    cond->typeCheck(typeBoolean);
    if (stmt1 != nullptr)
        stmt1->sem();
    if (stmt2 != nullptr)
        stmt2->sem();
}

llvm::Value *If::compile()
{
    // Compile condition
    llvm::Value *v = cond->compile();

    // Get the current function
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for 'then', 'else', and 'endif' sections
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(TheContext, "else", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "endif", TheFunction);

    // Create a conditional branch based on the condition value
    Builder.CreateCondBr(v, ThenBB, ElseBB);

    // Set insertion point to 'then' block and compile the 'then' statement
    Builder.SetInsertPoint(ThenBB);
    if (stmt1 != nullptr)
        stmt1->compile();

    // Create a branch to 'endif' block if 'then' block has no terminator
    if (!Builder.GetInsertBlock()->getTerminator())
        Builder.CreateBr(AfterBB); 

    // Set insertion point to 'else' block and compile the 'else' statement
    Builder.SetInsertPoint(ElseBB);
    if (stmt2 != nullptr)
        stmt2->compile();

    // Create a branch to 'endif' block if 'else' block has no terminator
    if (!Builder.GetInsertBlock()->getTerminator())
        Builder.CreateBr(AfterBB);

    // Set insertion point to 'endif' block
    Builder.SetInsertPoint(AfterBB);

    // Return nullptr since an if statement does not produce a value
    return nullptr;
}
