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
    llvm::Value* compile(std::vector<std::string> * signature_mangled_names, std::vector<llvmType*> * signature_types);

    IdList * getIdList();
    PassMode getPassMode();

    void addCapturedParameters(std::vector<std::string> *param_names, std::vector<llvmType*> *param_types, std::vector<bool> *ref);

private:
    IdList *idlist;
    FParType *fpartype;
    Type type;
    PassMode pass_mode;
    bool ref;

    SymbolEntry *function;
    unsigned int scope;
};

#endif