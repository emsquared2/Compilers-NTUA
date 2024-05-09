#ifndef __NOTHING_HPP__
#define __NOTHING_HPP__

#include "expr.hpp"
class Nothing : public Expr
{
public:
    Nothing();
    ~Nothing();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() const override;
};

#endif