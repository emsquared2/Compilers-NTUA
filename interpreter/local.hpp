#ifndef __LOCAL_HPP__
#define __LOCAL_HPP__

#pragma once

class Local : public AST
{
public:
    Local(FuncDef *fdef);
    Local(FuncDecl *fdecl);
    Local(Decl *decl);
    ~Local();
    virtual void printOn(std::ostream &out) const override;

private:
    FuncDef *funcdef;
    FuncDecl *funcdecl;
    Decl *decl;
};

#endif