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
    virtual llvm::Value* compile() override;

    virtual void setOuterFunction(std::string outer_func_name) override {};
    void addCapturedVariables(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref);

private:
    IdList *idlist;
    FParType *parser_type;
    Type type;
    std::vector<std::string> mangled_names;
};

#endif