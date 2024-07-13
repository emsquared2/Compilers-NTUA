#include "if.hpp"

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

void If::sem() 
{
    cond->type_check(typeBoolean);
    if (stmt1 != nullptr)
        stmt1->sem();
    if (stmt2 != nullptr)
        stmt2->sem();
}
