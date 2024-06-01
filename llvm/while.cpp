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
    cond->type_check(typeBoolean);
    stmt->sem();
}

llvm::Value *While::compile()
{
    // Grab the current function
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for different paths
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "while_loop", TheFunction);
    llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(TheContext, "while_body", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "endwhile", TheFunction);

    // Branch to the loop block
    Builder.CreateBr(LoopBB);

    // Set insertion point at the beginning of loop block
    Builder.SetInsertPoint(LoopBB);

    // Compile the condition
    llvm::Value *CondV = cond->compile();

    // Conditional branch based on the condition
    Builder.CreateCondBr(CondV, BodyBB, AfterBB);

    // Set insertion point at the beginning of body block
    Builder.SetInsertPoint(BodyBB);

    // Compile the statement inside the loop
    stmt->compile();

    if (!Builder.GetInsertBlock()->getTerminator())
        // Branch back to the loop block
        Builder.CreateBr(LoopBB);

    // Set insertion point at the end of while loop
    Builder.SetInsertPoint(AfterBB);

    // Return null pointer as this function does not return anything
    return nullptr;
}
