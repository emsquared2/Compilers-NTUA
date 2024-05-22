#include "block.hpp"

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
std::vector<Stmt *> Block::getList() { return stmt_list; }

void Block::sem()
{
    for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
    {
        (*s)->sem();
    }
}

llvm::Value* Block::compile()
{
    for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
    {
        (*s)->compile();

        // If statement s is a return statement, then do not compile anything else in the block.
        if (dynamic_cast<Return *> (*s))
            break;
    }
    return nullptr;
}