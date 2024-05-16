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
    std::string getMangledName() const;
    virtual void sem() override;
    virtual llvm::Value *compile() override;
    virtual llvm::Value *compile_ptr() const override;

private:
    std::string name;
};

#endif