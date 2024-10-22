#include "block.hpp"

Block::Block() : stmt_list() {}
Block::~Block() {
    for (Stmt *s : stmt_list)
        delete s;
}
void Block::append(Stmt *s) 
{ 
    stmt_list.push_back(s); 
}
void Block::printOn(std::ostream &out) const
{
    out << "Block(";
    bool first = true;
    for (Stmt *s : getReversed(stmt_list))
    {
        if (!first)
            out << ", ";
        first = false;
        out << *s;
    }
    out << ")";
}
std::vector<Stmt *> Block::getList() 
{ 
    return getReversed(stmt_list); 
}

void Block::sem()
{
    for (Stmt *s : getReversed(stmt_list))
        s->sem();
}

llvm::Value* Block::compile()
{
    for (Stmt *s : getReversed(stmt_list))
    {
        s->compile();

        /*
         * If the current statement is a return statement, terminate the loop.
         * This ensures that any statements after the return statement are not compiled,
         * as they would be unreachable in the actual program execution.
         */
        if (dynamic_cast<Return *> (s))
            break;
    }
    return nullptr;
}