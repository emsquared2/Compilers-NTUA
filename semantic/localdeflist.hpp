#ifndef __LOCALDEFLIST_HPP__
#define __LOCALDEFLIST_HPP__

#pragma once
#include <vector>

class LocalDefList : public AST
{
public:
    LocalDefList();
    ~LocalDefList();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void append(LocalDef *l);

private:
    std::vector<LocalDef *> locals;
};

#endif
