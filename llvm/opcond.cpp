#include "opcond.hpp"

OpCond::OpCond(Cond *l, const char *s, Cond *r) : left(l), op(s), right(r) {}

OpCond::OpCond(const char *s, Cond *r) : left(), op(s), right(r) {}

OpCond::~OpCond()
{
    if (left != nullptr)
        delete left;
    delete right;
}

void OpCond::printOn(std::ostream &out) const
{
    out << op << "(";
    if (left != nullptr)
        out << *left << ", ";
    out << *right << ")";
}

void OpCond::sem()
{
    if (left != nullptr)
        left->typeCheck(typeBoolean);
    right->typeCheck(typeBoolean);

    type = typeBoolean;
}

llvm::Value *OpCond::compile()
{
    llvm::Value *L = nullptr;
    llvm::Value *R = nullptr;
    std::string opStr = std::string(op);

    // Case for unary operator (e.g., "not")
    if (left == nullptr) {
        R = right->compile();
        if (!R) 
            return nullptr;

        if (opStr == "not")
            return Builder.CreateNot(R, "nottemp");
        else
            return LogErrorV("Invalid unary operator in opcond");
    }
    // Case for binary operators (e.g., "and", "or")
    else {
        L = left->compile();
        if (!L)
            return nullptr;

        llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *EvaluateRightBB = llvm::BasicBlock::Create(TheContext, "evalright", TheFunction);
        llvm::BasicBlock *SkipRightBB = llvm::BasicBlock::Create(TheContext, "skipright", TheFunction);
        llvm::BasicBlock *CurrentBB = Builder.GetInsertBlock();

        /* Case:
         *      and:    If left is false, then we can skip the compilation of right 
         *      or:     If left is true, then we can skip the compilation of right 
         */
       
        // Create conditional branches based on the operator
        if (opStr == "and")
            Builder.CreateCondBr(L, EvaluateRightBB, SkipRightBB);
        else if (opStr == "or")
            Builder.CreateCondBr(L, SkipRightBB, EvaluateRightBB);
        else
            return LogErrorV("Invalid binary operator in opcond");

        // Set insertion point to 'EvaluateRightBB' and compile the right condition if necessary
        Builder.SetInsertPoint(EvaluateRightBB);
        R = right->compile();
        if (!R)
            return nullptr;

        // Get the result of the binary operation
        llvm::Value *Result = nullptr;
        if (opStr == "and")
            Result = Builder.CreateAnd(L, R, "andtemp");
        else if (opStr == "or")
            Result = Builder.CreateOr(L, R, "ortemp");

        llvm::BasicBlock *AfterRightBB = Builder.GetInsertBlock();
        Builder.CreateBr(SkipRightBB);

        // Set insertion point to 'SkipRightBB'
        Builder.SetInsertPoint(SkipRightBB);
        // Create PHI node to select the correct result
        llvm::PHINode *PhiNode = Builder.CreatePHI(L->getType(), 2, "result");
        
        // Add incoming value from the current block (left condition result)
        PhiNode->addIncoming(L, CurrentBB);
        // Add incoming value from the block after evaluating the right condition
        PhiNode->addIncoming(Result, AfterRightBB);

        return PhiNode;
    }
}