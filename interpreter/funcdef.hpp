#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

class FuncDef : public AST
{
public:
    FuncDef(Header *h, LocalDef *ld, Stmt *s);
    ~FuncDef();
    virtual void printOn(std::ostream &out) const override;

private:
    Header *header;
    LocalDef *localdef;
    // Block *block;
    Stmt *stmt;
};

#endif