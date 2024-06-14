#ifndef __ID_HPP__
#define __ID_HPP__

#include "lvalue.hpp"
#include <string>

class Id : public LValue
{
public:
    Id(std::string s);
    virtual void printOn(std::ostream &out) const override;
    virtual const char *getName() const override;
    virtual void sem() override;
    virtual llvm::Value *compile() override;
    virtual llvm::Value *compile_ptr() override;
    virtual llvm::Value *compile_arr(std::vector<llvm::Value*> *offsets, llvmType ** t) override;
    llvm::Value *getCapturedVarAddr();

private:
    std::string name;
    std::string mangled_name;

    unsigned int decl_depth;
    unsigned int usage_depth;
    bool is_captured = false;

};

#endif