#include "while.hpp"

While::While(Cond *c, Stmt *s) : cond(c), stmt(s) {}

While::~While()
{
    delete cond;
    delete stmt;
}

void While::printOn(std::ostream &out) const
{
    out << "While(" << *cond << ", " << *stmt << ")";
}

void While::sem()
{
    cond->typeCheck(typeBoolean);
    stmt->sem();
}

llvm::Value *While::compile()
{
    // Get the current function
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for the loop condition, loop body, and after the loop
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "while_loop", TheFunction);
    llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(TheContext, "while_body", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "endwhile", TheFunction);

    // Branch to the loop block
    Builder.CreateBr(LoopBB);

    // Set the insertion point to the loop condition block
    Builder.SetInsertPoint(LoopBB);

    // Compile the loop condition
    llvm::Value *CondV = cond->compile();

    /* Create a conditional branch based on the condition value
     * If the condition is true, branch to the loop body block
     * If the condition is false, branch to the block after the loop */
    Builder.CreateCondBr(CondV, BodyBB, AfterBB);

    // Set the insertion point to the loop body block
    Builder.SetInsertPoint(BodyBB);

    // Compile the statement inside the loop body
    stmt->compile();

    // If the body block does not already have a terminator, create a branch back to the loop condition block
    if (!Builder.GetInsertBlock()->getTerminator())
        // Branch back to the loop block
        Builder.CreateBr(LoopBB);

    // Set the insertion point to the block after the loop
    Builder.SetInsertPoint(AfterBB);

    // Return nullptr since a while loop does not produce a value
    return nullptr;
}
