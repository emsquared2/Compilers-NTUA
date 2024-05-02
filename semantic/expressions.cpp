#include "expressions.hpp"

/* ---------------------------------------------------------------------
   ------------------------------- UnOp --------------------------------
   --------------------------------------------------------------------- */

UnOp::UnOp(char *s, Expr *e) : op(s), right(e) {}

UnOp::~UnOp()
{
    delete right;
}

void UnOp::printOn(std::ostream &out) const
{
    out << op << "(" << *right << ")";
}

void UnOp::sem()
{
    std::cout << "Unop Sem..." << std::endl;
    right->type_check(typeInteger);
    type = typeInteger;
}

/* ---------------------------------------------------------------------
   ------------------------------- BinOp -------------------------------
   --------------------------------------------------------------------- */

BinOp::BinOp(Expr *l, char *s, Expr *r) : left(l), op(s), right(r) {}

BinOp::~BinOp()
{
    delete left;
    delete right;
}

void BinOp::printOn(std::ostream &out) const
{
    out << op << "(" << *left << ", " << *right << ")";
}

void BinOp::sem()
{
    std::cout << "BinOp Sem..." << std::endl;
    left->type_check(typeInteger);
    right->type_check(typeInteger);
    type = typeInteger;
}


/* #####################################################################
   #####                        CONST TYPES                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- Const -------------------------------
   --------------------------------------------------------------------- */


Const::Const(int v) : val(v) {}
Const::~Const() {}
void Const::printOn(std::ostream &out) const
{
    out << "Const(" << val << ")";
}

void Const::sem()
{
    std::cout << "Const Sem..." << std::endl;
    type = typeInteger;
}


/* ---------------------------------------------------------------------
   ----------------------------- ConstChar -----------------------------
   --------------------------------------------------------------------- */

ConstChar::ConstChar(char v) : var(v) {}
ConstChar::~ConstChar() {}
void ConstChar::printOn(std::ostream &out) const
{
    out << "ConstChar(" << var << ")";
}
// virtual int eval() const override
// {
//     // return var;
// }
void ConstChar::sem()
{
    std::cout << "ConstChar Sem..." << std::endl;
    type = typeChar;
}

/* ---------------------------------------------------------------------
   ------------------------------ Nothing ------------------------------
   --------------------------------------------------------------------- */

Nothing::Nothing() {}
Nothing::~Nothing() {}
void Nothing::printOn(std::ostream &out) const
{
    out << "Nothing";
}
void Nothing::sem()
{
    std::cout << "Nothing Sem..." << std::endl;
    type = typeVoid;
}

/* ---------------------------------------------------------------------
   ---------------------- FUNCTION CALL EXPRESSION ---------------------
   --------------------------------------------------------------------- */

CallExpr::CallExpr(Id *id, ExprList *e_list = nullptr) : id(id), expr_list(e_list) {}
CallExpr::~CallExpr()
{
    delete id;
    delete expr_list;
}
void CallExpr::printOn(std::ostream &out) const
{
    out << "CallExpr(" << *id << "(";
    if (expr_list != nullptr)
        out << *expr_list;
    out << "))";
}
void CallExpr::sem()
{
    std::cout << "CallExpr Sem..." << std::endl;
    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);

    if(function->entryType != ENTRY_FUNCTION)
        SemanticError("This identifier does not correspond to a function.");

    // Check if function is called with correct number and type of arguments
    SymbolEntry *argument = function->u.eFunction.firstArgument;

    std::vector<Expr *> e_list = expr_list->getExprList();

    int counter = 0;

    for(Expr *e : e_list)
    {
      if(argument == NULL) {
        std::string msg = "Expected " + std::to_string(counter) + " arguments, but got " + std::to_string(e_list.size()) + ".";
        SemanticError(msg.c_str());
      }

      e->type_check(argument->u.eParameter.type);
      argument = argument->u.eParameter.next;

      counter++;
    }

    if(argument != NULL)
      SemanticError("Fewer parameters than expected in function call.");
    
    type = function->u.eFunction.resultType;
}