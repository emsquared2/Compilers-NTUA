#ifndef __LOCALDEFLIST_HPP__
#define __LOCALDEFLIST_HPP__

#include "localdef.hpp"
#include <vector>

class LocalDefList : public AST
{
public:
    LocalDefList();
    ~LocalDefList();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void append(LocalDef *l);
    virtual llvm::Value* compile() const override;

    std::vector<LocalDef*> getLocals();

private:
    std::vector<LocalDef *> locals;
};

#endif