#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

#include "statements.hpp"
#pragma once

class FuncDef : public AST
{
public:
    FuncDef(Header *h, LocalDefList *ldl, Block *b);
    ~FuncDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Header *header;
    LocalDefList *local_def_list;
    Block *block;
};

#endif