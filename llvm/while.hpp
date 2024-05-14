#ifndef __WHILE_HPP__
#define __WHILE_HPP__


#include "stmt.hpp"
#include "cond.hpp"

class While : public Stmt
{
public:
    While(Cond *c, Stmt *s);
    ~While();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    Cond *cond;
    Stmt *stmt;
};

#endif