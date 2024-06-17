#ifndef __CALLSTMT_HPP__
#define __CALLSTMT_HPP__

#include "stmt.hpp"
#include "id.hpp"
#include "exprlist.hpp"
#include "conststr.hpp"
#include "arrayelem.hpp"

class CallStmt : public Stmt
{
public:
    CallStmt(Id *id, ExprList *e_list);
    ~CallStmt();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual llvm::Value * compile() override;

private:
    Id *id;
    ExprList *expr_list = nullptr;
    std::vector<bool> ref;
    std::string mangled_name;

    unsigned int caller_depth;
    unsigned int callee_depth;
};

#endif