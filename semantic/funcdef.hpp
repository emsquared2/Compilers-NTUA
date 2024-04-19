#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

#pragma once

class FuncDef : public AST
{
public:
    // FuncDef(Header *h, LocalDef *ld, Stmt *s);
    FuncDef(Header *h, LocalDef *ld, Block *b);
    ~FuncDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Header *header;
    LocalDef *localdef;
    Block *block;
    // Stmt *stmt;
};

#endif