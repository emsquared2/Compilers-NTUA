#include "ast.hpp"

/* ---------------------------------------------------------------------
   ----------------------------- ConstStr ------------------------------
   --------------------------------------------------------------------- */

ConstStr::ConstStr(std::string s) : str(s) {}
void ConstStr::printOn(std::ostream &out) const
{
    out << "ConstStr(" << str << ")";
}
const char *ConstStr::getStr()
{
    return str.c_str();
}
const char *ConstStr::getName() const
{
    return str.c_str();
}
int ConstStr::eval() const
{
}
void ConstStr::sem()
{
    std::cout << "ConstStr Sem..." << std::endl;
    RepInteger len = str.length() + 1;
    type = typeArray(len, typeChar);
}

/* ---------------------------------------------------------------------
   -------------------------------- Id ---------------------------------
   --------------------------------------------------------------------- */

Id::Id(std::string s) : name(s) {}
void Id::printOn(std::ostream &out) const
{
    out << "Id(" << name << ")";
}
std::string Id::getId()
{
    return name.c_str();
}
const char *Id::getName() const
{
    return name.c_str();
}
int Id::eval() const
{
    // return rt_stack[offset]
}
void Id::sem()
{
    std::cout << "Id Sem..." << std::endl;
    SymbolEntry *e = lookupEntry(this->getName(), LOOKUP_ALL_SCOPES, true);

    switch (e->entryType)
    {
        case ENTRY_VARIABLE:
            type = e->u.eVariable.type;
            break;
        case ENTRY_CONSTANT:
            SemanticError("Id cannot be a constant.");
            break;
        case ENTRY_FUNCTION:
            SemanticError("Id cannot be a function.");
            break;    
        case ENTRY_PARAMETER:
            type = e->u.eParameter.type;
            break;
        case ENTRY_TEMPORARY:
            type = e->u.eTemporary.type;
            break;
        default:
            SemanticError("Invalid entry for identifier. This should never be reached");
            break;
    }
}

/* ---------------------------------------------------------------------
   ------------------------------ IdList -------------------------------
   --------------------------------------------------------------------- */

IdList::IdList() : idlist() {}
IdList::~IdList()
{
    for (Id *id : idlist)
    {
        delete id;
    }
}

const std::vector<Id *> IdList::get_idlist() { return idlist; }

void IdList::append(Id *id)
{
    idlist.push_back(id);
}
void IdList::printOn(std::ostream &out) const
{
    out << "IdList(";
    bool first = true;
    // for (Id *id : idlist)
    for (auto id = idlist.rbegin(); id != idlist.rend(); ++id)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **id;
    }
    out << ")";
}
void IdList::sem()
{
    std::cout << "IdList Sem..." << std::endl;
    for (Id *i : idlist)
        i->sem();
}

/* ---------------------------------------------------------------------
   ----------------------------- ExprList ------------------------------
   --------------------------------------------------------------------- */

ExprList::ExprList() : expr_list() {}
ExprList::~ExprList()
{
    for (Expr *expr : expr_list)
    {
        delete expr;
    }
}
void ExprList::append(Expr *e) { expr_list.push_back(e); }
void ExprList::printOn(std::ostream &out) const
{
    out << "ExprList(";
    bool first = true;
    // for (Expr *e : expr_list)
    for (auto e = expr_list.rbegin(); e != expr_list.rend(); ++e)
    {
        if (!first)
            out << ", ";
        first = false;
        out << **e;
    }
    out << ")";
}
std::vector<Expr *> ExprList::getExprList()
{
    return expr_list;
}
bool ExprList::isEmpty()
{
    return expr_list.empty();
}
void ExprList::sem()
{
    std::cout << "ExprList Sem..." << std::endl;
    for (Expr *e : expr_list)
        e->sem();
}

/* ---------------------------------------------------------------------
   ----------------------------- StmtList ------------------------------
   --------------------------------------------------------------------- */

StmtList::StmtList() : stmt_list() {}
StmtList::~StmtList()
{
    for (Stmt *stmt : stmt_list)
    {
        delete stmt;
    }
}
void StmtList::append(Stmt *s) { stmt_list.push_back(s); }
void StmtList::printOn(std::ostream &out) const
{
    out << "StmtList(";
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
void StmtList::sem()
{
    std::cout << "StmtList Sem..." << std::endl;
    for (Stmt *s : stmt_list)
        s->sem();
}

/* ---------------------------------------------------------------------
   ------------------------------ FParam -------------------------------
   --------------------------------------------------------------------- */

FParam::FParam(IdList *idl, FParType *t, bool ref = false) : idlist(idl), fpartype(t), ref(ref)
{
    pass_mode = ref ? PASS_BY_REFERENCE : PASS_BY_VALUE;
    type = fpartype->ConvertToType(fpartype);
}
FParam::~FParam()
{
    delete idlist;
    delete fpartype;
    if (fpartype)
    {
        destroyType(type);
    }
}
void FParam::printOn(std::ostream &out) const
{
    out << "FParam(";
    if (ref)
        out << "ref ";
    out << *idlist << " : ";
    out << *fpartype;
    // if (fpartype) printType(fpartype);
    out << ")";
}

void FParam::sem()
{
    std::cout << "FParam Sem..." << std::endl;
    for (Id *id : idlist->get_idlist())
        SymbolEntry *fun_param = newParameter(id->getName(), type, pass_mode, function);
}

void FParam::setSymbolEntry(SymbolEntry *f)
{
    function = f;
}

/* ---------------------------------------------------------------------
   ---------------------------- FParamList -----------------------------
   --------------------------------------------------------------------- */

FParamList::FParamList() : params() {}
FParamList::~FParamList()
{
    for (FParam *param : params)
    {
        delete param;
    }
}
void FParamList::printOn(std::ostream &out) const
{
    bool first = true;
    out << "FParamList(";
    for (auto param = params.rbegin(); param != params.rend(); ++param)
    {
        if (!first)
            out << "; ";
        first = false;
        out << **param;
    }
    out << ")";
}

// Check class Header for information about this sem() method.

void FParamList::sem()
{
    std::cout << "FParamList Sem..." << std::endl;
    for (FParam *param : params)
    {
        param->setSymbolEntry(function);
        param->sem();
    }
}

void FParamList::append(FParam *param)
{
    params.push_back(param);
}

void FParamList::setSymbolEntry(SymbolEntry *f)
{
    function = f;
}


/* ---------------------------------------------------------------------
   ------------------------------- Decl --------------------------------
   --------------------------------------------------------------------- */

Decl::Decl(IdList *idl, FParType *fpt) : idlist(idl), parser_type(fpt) { type = parser_type->ConvertToType(parser_type); }
Decl::~Decl()
{
    delete idlist;
    delete parser_type;
    if (type)
    {
        destroyType(type);
    }
}
void Decl::printOn(std::ostream &out) const
{
    out << "Decl(var " << *idlist << " : " << *parser_type;
    // if(type) { printType(type); }
    out << ")";
}
void Decl::sem()
{
    std::cout << "Decl Sem..." << std::endl;
    for (Id *id : idlist->get_idlist())
    {
        // lookupEntry(id->getName(), LOOKUP_CURRENT_SCOPE, true);
        newVariable(id->getName(), type);
    }
}

/* ---------------------------------------------------------------------
   ------------------------------ Header -------------------------------
   --------------------------------------------------------------------- */

Header::Header(Id *id, RetType *t, FParamList *fpl = nullptr) : id(id), ret_type(t), fparamlist(fpl) { type = ret_type->ConvertToType(ret_type); }
Header::~Header()
{
    delete id;
    if (fparamlist != nullptr)
    {
        delete fparamlist;
    }
    delete ret_type;
    if (type)
    {
        destroyType(type);
    }
}
void Header::printOn(std::ostream &out) const
{
    out << "Header(fun " << *id << "(";
    if (fparamlist != nullptr)
        out << *fparamlist;
    out << ") : ";
    out << *ret_type;
    // if(type) printType(type);
    out << ")";
}
void Header::sem()
{
    /* Check if id exists in symbol table. If it exists, throw error.
     * If not:
     *  1) add it to the symbol table
     *  2) open a new scope
     *  3) store the return type
     *  4) paramlit->sem()
     *  5) close the scope?
     */

    std::cout << "Header Sem..." << std::endl;

    SymbolEntry *function = newFunction(id->getName());

    // add case when parser identifies forward declaration
    if (forward_declaration)
        forwardFunction(function);

    openScope();
    /*
    Here we have to add all the parameters to the symbol entry.
    The SymbolEntry of the function (look *function above) is also needed when creating a new function parameter.
    We have 2 options:
    Option 1: Pass the function SymbolEntry to the ParamList and handle the sem() function inside the ParamList class
    Option 2: Add a method to the ParamList class that returns the private vector of Parameters. Then handle each Param in this sem() function.

    Option 1 might be easier to implement but having a field of SymbolEntry on a Class might seem a little be counterintuitive.

    Option 2 on the other hand might be a cleaner option as the SymbolEntry is abstracted from the classes.
    */
    if (fparamlist != nullptr) {
        fparamlist->setSymbolEntry(function);
        fparamlist->sem();

    }

    endFunctionHeader(function, type);
}

void Header::set_forward_declaration()
{
    forward_declaration = true;
}

/* ---------------------------------------------------------------------
   ----------------------------- FuncDecl ------------------------------
   --------------------------------------------------------------------- */

FuncDecl::FuncDecl(Header *h) : header(h) {}
FuncDecl::~FuncDecl() { delete header; }
void FuncDecl::printOn(std::ostream &out) const
{
    out << "FuncDecl(" << *header << ")";
}
void FuncDecl::sem()
{
    std::cout << "FuncDecl Sem..." << std::endl;
    // FuncDecl is for forward declaring function.
    // Header class is for both forward and non forward functions.
    // Set forward_declaration to true to specify a header that represents a forward declared function.
    header->set_forward_declaration();
    header->sem();

    closeScope();
}

/* ---------------------------------------------------------------------
   ----------------------------- ArrayElem -----------------------------
   --------------------------------------------------------------------- */

ArrayElem::ArrayElem(LValue *l, ExprList *e) : left(l), exprlist(e) {}
ArrayElem::~ArrayElem()
{
    delete left;
    delete exprlist;
}
void ArrayElem::printOn(std::ostream &out) const
{
    std::vector<Expr *> exprs = exprlist->getExprList();
    out << "ArrayElem(" << *left;
    for (auto e = exprs.rbegin(); e != exprs.rend(); ++e)
    {
        out << "[" << **e << "]";
    }
    out << ")";
}
std::string ArrayElem::getArrayElem()
{
    // return left->getName() + "[" + std::to_string(expr->eval()) + "]";
}
int ArrayElem::eval() const
{
}
void ArrayElem::sem()
{
    std::cout << "ArrayElem Sem..." << std::endl;  
    std::vector<Expr *> expr_list = exprlist->getExprList();

    left->sem();

    for(Expr *e : expr_list) {
        e->type_check(typeInteger);
        e->sem();
    }

    // What else?
    
}