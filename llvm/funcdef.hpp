#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

#include "localdef.hpp"
#include "header.hpp"
#include "block.hpp"
#include "localdeflist.hpp"


class FuncDef : public LocalDef
{
public:
    FuncDef(Header *h, LocalDefList *ldl, Block *b);
    ~FuncDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void ProgramSem();
    virtual llvm::Function *compile() override;
    void optimizeFunc(llvm::Function *function);

private:
    Header *header;
    LocalDefList *local_def_list;
    Block *block;
    std::string mangled_name;
};

#endif