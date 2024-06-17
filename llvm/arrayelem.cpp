#include "arrayelem.hpp"

ArrayElem::ArrayElem(LValue *l, ExprList *e) : left(l), exprlist(e) {}
ArrayElem::~ArrayElem()
{
    delete left;
    delete exprlist;
}
void ArrayElem::printOn(std::ostream &out) const
{
    out << "ArrayElem(" << *left;
    for (Expr *e : exprlist->getExprList())
    {
        out << "[" << *e << "]";
    }
    out << ")";
}

void ArrayElem::sem()
{
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for (Expr *e : expr_list)
    {
        e->typeCheck(typeInteger);
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


llvm::Value * ArrayElem::compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t)
{
    for (Expr *e :  exprlist->getExprList()) {
        llvm::Value *off = e->compile();
        *t = getLLVMType(e->getType(), TheContext);
        if(!off)
            return nullptr;
        offsets->push_back(off);
    }
    return left->compile_arr(offsets, t);
}

llvm::Value* ArrayElem::compile_ptr() {
    std::vector<llvm::Value*> offsets;
    llvmType* elementType;
    llvm::Value* baseAddr = compile_arr(&offsets, &elementType);
    if (!baseAddr)
        return nullptr;

    // Get the pointer type from the LLVM type of the element
    llvm::PointerType* ptrType = llvm::PointerType::get(elementType, 0); // Assuming the first level of pointer (change if different levels are used)

    // Generate a pointer to the element by applying the offsets to the base address
    llvm::Value* elementPtr = Builder.CreateGEP(ptrType, baseAddr, offsets);
    return elementPtr;
}

llvm::Value* ArrayElem::compile() 
{
    llvm::Value* Addr = compile_ptr();
    if (!Addr)
        return LogErrorV("ArrayElem Compile: Execution shouldn't reach this point.");
        
    Type t = findArrayType(left->getType());
    return Builder.CreateLoad(getLLVMType(t, TheContext), Addr);    
}