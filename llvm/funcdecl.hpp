#ifndef __FUNCDECL_HPP__
#define __FUNCDECL_HPP__

#include "localdef.hpp"
#include "header.hpp"

class FuncDecl : public LocalDef
{
public:
    FuncDecl(Header *h);
    ~FuncDecl();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    Header *header;
};
#endif