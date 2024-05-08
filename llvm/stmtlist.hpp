#ifndef __STMTLIST_HPP__
#define __STMTLIST_HPP__

#include "stmt.hpp"
#include <vector>

class StmtList : public AST
{
public:
    StmtList();
    ~StmtList();
    void append(Stmt *s);
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value* compile() const override;

private:
    std::vector<Stmt *> stmt_list;
};

#endif