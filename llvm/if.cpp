#include "if.hpp"

If::If(Cond *c, Stmt *s1, Stmt *s2=nullptr) : cond(c), stmt1(s1), stmt2(s2) {}

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

llvm::Value* If::compile() const
{
    // Compile condition
    llvm::Value *CondV = cond->compile();

    // Convert condition to a bool by comparing non-equal to 0.0.
    CondV = Builder.CreateFCmpONE(
    CondV, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "ifcond");

    // Grab current function
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // Create basic blocks for different paths
    llvm::BasicBlock *ThenBB  = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB  = llvm::BasicBlock::Create(TheContext, "else", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "endif", TheFunction);
  
    // Branch based on the condition
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Set insertion point for the 'then' block
    Builder.SetInsertPoint(ThenBB);
    
    // Compile statement inside the 'then' block
    llvm::Value *ThenV = stmt1->compile();
    if (!ThenV)
        return nullptr;
    
    // Create branch to the 'endif' block
    Builder.CreateBr(AfterBB);

    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder.GetInsertBlock();

    // Emit else block.
    // TheFunction->insert(TheFunction->end(), ElseBB);

    // Set insertion point for the 'else' block
    Builder.SetInsertPoint(ElseBB);

    // Compile statement inside the 'else' block, if it exists
    if (stmt2 != nullptr) 
        return nullptr;
    llvm::Value *ElseV = stmt2->compile();
    if (!ElseV)
        return nullptr;

    // Create branch to the 'endif' block if 'else' block exists or conditionally if not
    Builder.CreateBr(AfterBB);

    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder.GetInsertBlock();

    // Emit after block.
    // TheFunction->insert(TheFunction->end(), AfterBB);
    // Set insertion point for the 'endif' block
    Builder.SetInsertPoint(AfterBB);
    llvm::PHINode *PN = Builder.CreatePHI(llvmType::getDoubleTy(TheContext), 2, "iftmp");
        
    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    
    return PN;
}
