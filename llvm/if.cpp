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
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr)
        out << " Else " << *stmt2;
    out << ")";
}

void If::sem()
{
    cond->type_check(typeBoolean);
    stmt1->sem();
    if (stmt2 != nullptr)
        stmt2->sem();
}

llvm::Value *If::compile()
{
    // Compile condition
    llvm::Value *v = cond->compile();

    // Grab current function
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for different paths
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(TheContext, "else", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "endif", TheFunction);

    // Branch based on the condition
    Builder.CreateCondBr(v, ThenBB, ElseBB);

    // Set insertion point for the 'then' block
    Builder.SetInsertPoint(ThenBB);

    // Compile statement inside the 'then' block
    stmt1->compile();

    // Set insertion point for the 'else' block
    if (!Builder.GetInsertBlock()->getTerminator())
        Builder.CreateBr(AfterBB); // Create branch to the 'endif' block

    Builder.SetInsertPoint(ElseBB);

    // Compile statement inside the 'else' block, if it exists
    if (stmt2 != nullptr)
    {
        stmt2->compile();
    }


    // Create branch to the 'endif' block if 'else' block exists or conditionally if not
    // Branch should also exist on empty stmt2 to endif. Otherwise we have wrong code flow.
    if (!Builder.GetInsertBlock()->getTerminator())
        Builder.CreateBr(AfterBB);

    // Set insertion point for the 'endif' block

    Builder.SetInsertPoint(AfterBB);

    // Return null since If statement doesn't produce a value
    return nullptr;
}
