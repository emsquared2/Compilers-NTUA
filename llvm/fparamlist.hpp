#ifndef __FPARAMLIST_HPP__
#define __FPARAMLIST_HPP__

#include "fparam.hpp"
#include <vector>

class FParamList : public AST
{
public:
    FParamList();
    ~FParamList();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void append(FParam *param);
    void setSymbolEntry(SymbolEntry *f);
    virtual llvm::Value *compile() cost override;

private:
    std::vector<FParam *> params;
    SymbolEntry *function;
};

#endif