#include "while.hpp"

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

void While::sem() {
    std::cout << "While Sem..." << std::endl;
    cond->type_check(typeBoolean);
    stmt->sem();
}