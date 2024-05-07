#ifndef __CALLSTMT_HPP__
#define __CALLSTMT_HPP__

#include "stmt.hpp"
#include "id.hpp"
#include "exprlist.hpp"

class CallStmt : public Stmt
{
public:
    CallStmt(Id *id, ExprList *e_list);
    ~CallStmt();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    Id *id;
    ExprList *expr_list = nullptr;
};

#endif