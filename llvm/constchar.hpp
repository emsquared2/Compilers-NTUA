#ifndef __CONSTCHAR_HPP__
#define __CONSTCHAR_HPP__

#include "expr.hpp"

class ConstChar : public Expr
{
public:
    ConstChar(char v);
    ~ConstChar();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm:Value* compile() const override;

private:
    char var;
};

#endif