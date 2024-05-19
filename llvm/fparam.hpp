#ifndef __FPARAM_HPP__
#define __FPARAM_HPP__

#include "idlist.hpp"
#include "fpartype.hpp"

class FParam : public AST
{
public:
    FParam(IdList *idl, FParType *t, bool ref);
    ~FParam();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void setSymbolEntry(SymbolEntry *f);
    Type getType();
    llvm::Value *compile() override;

    IdList * getIdList();
    PassMode getPassMode();

private:
    IdList *idlist;
    FParType *fpartype;
    Type type;
    PassMode pass_mode;
    bool ref;

    SymbolEntry *function;
};

#endif