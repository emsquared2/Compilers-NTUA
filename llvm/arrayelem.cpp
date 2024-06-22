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

    type = findArrayType(t, exprlist->getExprList().size());
}

llvm::Value * ArrayElem::compile_arr(std::vector<llvm::Value*> *offsets, llvm::Type **t) {
    for (Expr *e : exprlist->getExprList()) {
        llvm::Value *off = e->compile();
        if (!off)
            return nullptr;
        offsets->push_back(off);
    }
    return left->compile_arr(offsets, t);
}

llvm::Value* ArrayElem::compile_ptr() {
    std::vector<llvm::Value*> offsets;
    llvm::Type* elementType;
    llvm::Value* baseAddr = compile_arr(&offsets, &elementType);
    if (!baseAddr)
        return nullptr;

    // Return a pointer to the specific array element by applying the offsets to the base address
    return Builder.CreateGEP(elementType, baseAddr, offsets);
}

llvm::Value* ArrayElem::compile() 
{
    llvm::Value* Addr = compile_ptr();
    if (!Addr)
        return LogErrorV("ArrayElem Compile: Execution shouldn't reach this point.");
    
    llvmType *elementType = getLLVMType(type, TheContext);
    return Builder.CreateLoad(elementType, Addr);    
}