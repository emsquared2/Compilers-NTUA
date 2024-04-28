#ifndef __LOCALDEF_HPP__
#define __LOCALDEF_HPP__

#pragma once
#include <vector>

class LocalDef : public AST
{
public:
    LocalDef();
    ~LocalDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void append(Local *l);

private:
    std::vector<Local *> locals;
};

#endif