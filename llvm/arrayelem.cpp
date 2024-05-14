#include "arrayelem.hpp"

ArrayElem::ArrayElem(LValue *l, ExprList *e) : left(l), exprlist(e) {}
ArrayElem::~ArrayElem()
{
    delete left;
    delete exprlist;
}
void ArrayElem::printOn(std::ostream &out) const
{
    std::vector<Expr *> exprs = exprlist->getExprList();
    out << "ArrayElem(" << *left;
    for (auto e = exprs.rbegin(); e != exprs.rend(); ++e)
    {
        out << "[" << **e << "]";
    }
    out << ")";
}
std::string ArrayElem::getArrayElem()
{
    // return left->getName() + "[" + std::to_string(expr->eval()) + "]";
}
void ArrayElem::sem()
{
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for (Expr *e : expr_list)
    {
        e->type_check(typeInteger);
        e->sem();
    }

    Type t = left->getType();
    if (t->refType == nullptr)
    {
        std::string name = left->getName();
        std::string msg = "Variable " + name + " is not of type Array";
        SemanticError(msg.c_str());
    }

    type = findArrayType(t);
}

llvm::Value *ArrayElem::compile_ptr() const
{
    return nullptr;
    // llvm::Value* basePtr = left->compile_ptr(); // Get the base pointer to the array

    // if (!basePtr)
    //     return nullptr;

    // // Ensure the pointer is indeed a pointer type
    // llvm::PointerType* ptrType = llvm::dyn_cast<llvm::PointerType>(basePtr->getType());
    // if (!ptrType)
    //     return LogErrorV("Base pointer is not a pointer type.");

    // std::vector<llvm::Value*> indices;
    // std::vector<Expr*> expr_list = exprlist->getExprList();
    
    // // LLVM arrays are zero-indexed, push 0 as the first index for the base pointer
    // indices.push_back(c64(0));
    
    // for (Expr* expr : expr_list) {
    //     llvm::Value* index = expr->compile();
    //     if (index) {
    //         indices.push_back(index);
    //     } else {
    //         return LogErrorV("Index expression compilation failed.");
    //     }
    // }

    // // Ensure that the basePtr points to an actual array
    // if (ptrType->getElementType()->isArrayTy() || ptrType->getElementType()->isStructTy()) {
    //     // Create the GEP instruction to calculate the element pointer
    //     llvm::Value* elementPtr = Builder.CreateGEP(ptrType->getElementType(), basePtr, indices);
    //     return elementPtr;
    // } else {
    //     return LogErrorV("Pointer type does not point to an array or struct.");
    // }
}

llvm::Value *ArrayElem::compile()
{
    return nullptr;
    // // Get the pointer to the element
    // llvm::Value* elementPtr = this->compile_ptr();

    // // Load the value from the pointer
    // if (elementPtr)
    //     return Builder.CreateLoad(elementPtr->getType()->getPointerElementType(), elementPtr);
    //     // return Builder.CreateLoad(getLLVMType(type, TheContext), elementPtr);
    // else
    //     return nullptr;
}
