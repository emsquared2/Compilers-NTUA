#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include "id.hpp"
#include "fparamlist.hpp"
#include "rettype.hpp"

class Header : public AST
{
public:
    Header(Id *id, RetType *t, FParamList *fpl);
    ~Header();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void set_forward_declaration();
    FParamList *getFParamList();
    Type getReturnType();
    Id *getId();
    virtual llvm::Function *compile() const override;

    std::vector<llvmType *> getLLVM_param_types();
    std::vector<std::string> getLLVM_param_names();

private:
    Id *id;
    RetType *ret_type;
    FParamList *fparamlist;
    Type type;
    bool forward_declaration = false;
};

#endif