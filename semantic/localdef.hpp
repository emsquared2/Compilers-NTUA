#ifndef __LOCALDEF_HPP__
#define __LOCALDEF_HPP__

#pragma once

class LocalDef : public AST
{
public:
    LocalDef(FuncDef *fdef);
    LocalDef(FuncDecl *fdecl);
    LocalDef(Decl *decl);
    ~LocalDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    FuncDef *funcdef;
    FuncDecl *funcdecl;
    Decl *decl;
};

#endif