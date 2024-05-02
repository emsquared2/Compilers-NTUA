#include "statements.hpp"

/* ---------------------------------------------------------------------
   ------------------------------- BLOCK -------------------------------
   --------------------------------------------------------------------- */

Block::Block() : stmt_list() {}
Block::~Block()
{
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
    std::cout << "Block Sem..." << std::endl;
    for (auto s = stmt_list.rbegin(); s != stmt_list.rend(); ++s)
    {
        (*s)->sem();
    }
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
    std::cout << "Assign Sem..." << std::endl;

    l_value->sem();
    expr->sem();

    // this probably will existin in l_value sem so it will not be needed here
    // lookupEntry(l_value->getName(), LOOKUP_ALL_SCOPES, true);

    Type l_value_type = l_value->getType();
    Type expr_type = expr->getType();

    // Check that types match
    if (!equalType(l_value_type, expr_type))
        SemanticError("Assign: L_value and Expr type mismatch.");

    // Check that their type is typeInteger or typeChar
    if (!equalType(l_value_type, typeInteger) && !equalType(l_value_type, typeChar))
        SemanticError("Assign: L_value's type should be typeInteger or typeChar");

    if (!equalType(expr_type, typeInteger) && !equalType(expr_type, typeChar))
        SemanticError("Assign: Expr's type should be typeInteger or typeChar");
}

/* ---------------------------------------------------------------------
   --------------------------------- IF --------------------------------
   --------------------------------------------------------------------- */

If::If(Cond *c, Stmt *s1, Stmt *s2 = nullptr) : cond(c), stmt1(s1), stmt2(s2) {}

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
    std::cout << "If Sem..." << std::endl;
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

void While::sem()
{
    std::cout << "While Sem..." << std::endl;
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
    std::cout << "Return Sem..." << std::endl;

    /* TODO: Get return type.
     * There are 2 options:
     *      a) Use a stack that keeps the return types (function decls are nested so you only need to check the last element).
     *      b) Implement a method in symbol.c in order to get the return type from the last function in scope.
     */

    SymbolEntry *func = lookupLastFunction();

    Type return_type = func->u.eFunction.resultType;

    // If return type is void, then expr should be nullptr
    if(equalType(return_type, typeVoid)) {
        if(expr != nullptr) {
            SemanticError("Return type should be void");
        }
    }
    else if(equalType(return_type, typeInteger) || equalType(return_type, typeChar))
    {
        if(expr != nullptr) 
            expr->type_check(return_type);
        else SemanticError("Return's expression type does not match return type.");
    }
    else {
        SemanticError("Return type should be int, char or void");
    }

}

/* ---------------------------------------------------------------------
   ------------------------------ EMPTY STMT----------------------------
   --------------------------------------------------------------------- */

EmptyStmt::EmptyStmt() {}

void EmptyStmt::printOn(std::ostream &out) const
{
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
    std::cout << "CallStmt Sem..." << std::endl;

    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);

    if (function->entryType != ENTRY_FUNCTION)
        SemanticError("Could not find function name.");

    // Check if function is called with correct number and type of arguments
    SymbolEntry *argument = function->u.eFunction.firstArgument;

    std::vector<Expr *> e_list;
    
    if (expr_list != nullptr)
        e_list = expr_list->getExprList();        

    int counter = 0;

    for (Expr *e : e_list)
    {
        // More parameters than expected
        if (argument == NULL)
        {
            std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
            SemanticError(msg.c_str());
        }

        e->type_check(argument->u.eParameter.type);
        argument = argument->u.eParameter.next;

        counter++;
    }

    // Fewer parameters than expected
    if (argument != NULL)
    {
        // Find true number of arguments
        while(argument != NULL) {
            argument = argument->u.eParameter.next;
            counter++;
        }
        
        std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
        SemanticError(msg.c_str());
    }


    /* I think this is the only difference:
     * CallStmt is not an Expr so it does not have a type.
     * Function statements should have void return type.
     */
    if(!equalType(function->u.eFunction.resultType, typeVoid))
        SemanticError("CallStmt: Function should have void return type");
}
