#ifndef __DECL_HPP__
#define __DECL_HPP__

#include "localdef.hpp"
#include "idlist.hpp"
#include "fpartype.hpp"

class Decl : public LocalDef
{
public:
    Decl(IdList *idl, FParType *fpt);
    ~Decl();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() const override;

private:
    IdList *idlist;
    FParType *parser_type;
    Type type;
};

#endif