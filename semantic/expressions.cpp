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
    // Check if the function exists
    SymbolEntry *function = lookupEntry(id->getName(), LOOKUP_ALL_SCOPES, true);
    // TODO: Check if function is called properly (number and type of arguments)
}