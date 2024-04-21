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
    void append(LocalDef *l);

private:
    std::vector<LocalDef *> locals;
};

#endif
