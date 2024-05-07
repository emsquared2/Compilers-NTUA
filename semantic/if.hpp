#ifndef __IF_HPP__
#define __IF_HPP__

#include "stmt.hpp"
#include "cond.hpp"

class If : public Stmt
{
public:
    If(Cond *c, Stmt *s1, Stmt *s2);
    ~If();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Cond *cond;
    Stmt *stmt1, *stmt2;
};

#endif