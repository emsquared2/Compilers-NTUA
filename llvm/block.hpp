#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include "stmt.hpp"
#include <vector>

class Block : public Stmt
{
public:
    Block();
    ~Block();
    void append(Stmt *s);
    virtual void printOn(std::ostream &out) const override;
    std::vector<Stmt *> getList();
    virtual void sem() override;
    virtual llvm::Value* compile() override;

private:
    std::vector<Stmt *> stmt_list;
};

#endif