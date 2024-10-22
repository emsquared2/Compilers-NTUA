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
    void setForwardDeclaration();
    FParamList *getFParamList();
    Type getReturnType();
    Id *getId();
    virtual llvm::Function *compile() override;

    std::vector<llvmType *> getLLVM_param_types();
    std::vector<std::string> getLLVM_param_names();
    std::string getHMangledName();

    void addStaticLinkToFunctionSignature(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types);
    void addCapturedParametersToSignature(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref);

private:
    Id *id;
    RetType *ret_type;
    FParamList *fparamlist;
    Type type;
    bool forward_declaration = false;

    std::string mangled_name;
    std::vector<llvmType*> llvm_param_types;
    std::vector<std::string> llvm_param_names;
};

#endif