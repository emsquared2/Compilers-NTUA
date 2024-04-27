#include "statements.hpp"

/* ---------------------------------------------------------------------
   ------------------------------- BLOCK -------------------------------
   --------------------------------------------------------------------- */

Block::Block() : stmt_list() {}
Block::~Block() {
    for (Stmt *s : stmt_list)
        delete s;
}
void Block::append(Stmt *s) { stmt_list.push_back(s); }
void Block::printOn(std::ostream &out) const
{
    out << "Block(";
    bool first = true;
    for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **s;
    }
    out << ")";
} 
void Block::run() const
{

}
std::vector<Stmt *> Block::getList() { return stmt_list; }

void Block::sem()
{
    
}

/* ---------------------------------------------------------------------
   ------------------------------- ASSIGN ------------------------------
   --------------------------------------------------------------------- */

Assign::Assign(LValue *l, Expr *e) : l_value(l), expr(e) {}

Assign::~Assign()
{
    delete l_value;
    delete expr;
}

void Assign::printOn(std::ostream &out) const
{
    out << "Assign(" << *l_value << " <- " << *expr << ")";
}

void Assign::run() const
{
   
}

void Assign::sem()
{

}


/* ---------------------------------------------------------------------
   --------------------------------- IF --------------------------------
   --------------------------------------------------------------------- */

If::If(Cond *c, Stmt *s1, Stmt *s2=nullptr) : cond(c), stmt1(s1), stmt2(s2) {}

If::~If() 
{
    delete cond;
    delete stmt1;
    delete stmt2;
}

void If::printOn(std::ostream &out) const 
{
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr)
        out << " Else " << *stmt2;
    out << ")";
}

void If::run() const 
{
    // if (cond->eval())
    //     stmt1->run();
    // else if (stmt2 != nullptr)
    //     stmt2->run();
}

void If::sem() 
{
    cond->type_check(typeBoolean);
    stmt1->sem();
    if (stmt2 != nullptr)
        stmt2->sem();
}

/* ---------------------------------------------------------------------
   -------------------------------- WHILE ------------------------------
   --------------------------------------------------------------------- */

While::While(Cond *c, Stmt *s) : cond(c), stmt(s) {}

While::~While() 
{
    delete cond;
    delete stmt;
}

void While::printOn(std::ostream &out) const 
{
    out << "While(" << *cond << ", " << *stmt << ")";
}

void While::run() const 
{
    // while (cond->eval())
    //     stmt->run();
}

void While::sem() {
    cond->type_check(typeBoolean);
    stmt->sem();
}

/* ---------------------------------------------------------------------
   ------------------------------- RETURN ------------------------------
   --------------------------------------------------------------------- */

Return::Return(Expr *e) : expr(e) {}

void Return::printOn(std::ostream &out) const 
{
    out << "Return(";
    if (expr != nullptr)
        out << *expr;
    out << ")";
}

void Return::run() const 
{

}

int Return::ReturnValue() 
{
    // return (expr != nullptr) ? expr->eval() : 0;
}

void Return::sem() {

}

/* ---------------------------------------------------------------------
   ------------------------------ EMPTY STMT----------------------------
   --------------------------------------------------------------------- */


EmptyStmt::EmptyStmt() {}

void EmptyStmt::printOn(std::ostream &out) const {
    out << "Empty Statement()";
}

/* ---------------------------------------------------------------------
   ---------------------- FUNCTION CALL STATEMENT ----------------------
   --------------------------------------------------------------------- */

CallStmt::CallStmt(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
CallStmt::~CallStmt()
{
    delete id;
    delete expr_list;
}
void CallStmt::printOn(std::ostream &out) const
{
    out << "CallStmt(" << *id << "(";
    if (expr_list != nullptr)
        out << *expr_list;
    out << "))";
}
void CallStmt::run() const
{

}
void CallStmt::sem()
{
    id->sem();
    expr_list->sem();
}