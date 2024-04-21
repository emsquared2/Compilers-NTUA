#include "ast.hpp"

/* ---------------------------------------------------------------------
   ----------------------------- ConstStr ------------------------------
   --------------------------------------------------------------------- */


ConstStr::ConstStr(std::string s) : str(s) {}
void ConstStr::printOn(std::ostream &out) const
{
    out << "ConstStr(" << str << ")";
}
const char * ConstStr::getStr()
{
    return str.c_str();
}
const char * ConstStr::getName() const 
{
    return str.c_str();
}
int ConstStr::eval() const 
{
}
void ConstStr::sem()  {
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
const char * Id::getName() const
{
    return name.c_str();
}
int Id::eval() const
{
    // return rt_stack[offset]
}
void Id::sem()
{
    // SymbolEntry *e = st.lookup(name);
    // if (e == nullptr)
    // {
    //     std::cerr << "Undeclared variable " << name << std::endl;
    //     throw std::runtime_error("Semantic error: Undeclared variable");
    // }
    // else
    // {
    //     type = e->getType();
    //     offset = e->getOffset();
    // }
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
    for (Stmt *s : stmt_list)
        s->sem();
}


/* ---------------------------------------------------------------------
   ------------------------------ FParam -------------------------------
   --------------------------------------------------------------------- */


FParam::FParam(IdList *idl, FParType *t, bool ref = false) : idlist(idl), fpartype(t), ref(ref) {
    pass_mode = ref ? PASS_BY_REFERENCE : PASS_BY_VALUE;
    type = fpartype->ConvertToType(fpartype);
}
FParam::~FParam() { 
    delete idlist;
    delete fpartype;
    if (fpartype) { destroyType(type); }
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
void FParamList::append(FParam *param)
{
    params.push_back(param);
}

/* ---------------------------------------------------------------------
   ------------------------------- Decl --------------------------------
   --------------------------------------------------------------------- */


Decl::Decl(IdList *idl, FParType *fpt) : idlist(idl), parser_type(fpt) { type = parser_type->ConvertToType(parser_type); }
Decl::~Decl()
{
    delete idlist;
    delete parser_type;
    if (type) { destroyType(type); }
}
void Decl::printOn(std::ostream &out) const
{
    out << "Decl(var " << *idlist << " : " << *parser_type;
    // if(type) { printType(type); }
    out << ")";
}
void Decl::sem()
{
    // for (Id *id : idlist->get_idlist())
    // {
    //     st.lookup(id->getId());
    //     st.insert(id->getId(), type);
    // }
}

/* ---------------------------------------------------------------------
   ------------------------------ Header -------------------------------
   --------------------------------------------------------------------- */

Header::Header(Id *id, RetType *t, FParamList *fpl = nullptr) : id(id), ret_type(t), fparamlist(fpl) { type = ret_type->ConvertToType(ret_type); }
Header::~Header()
{
    delete id;
    if (fparamlist != nullptr) { delete fparamlist; }
    delete ret_type;
    if (type) { destroyType(type); }
}
void Header::printOn(std::ostream &out) const
{
    out << "Header(fun " << *id << "(";
    if (fparamlist != nullptr)
        out << *fparamlist;
    out << ") : " ;
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
    // Add funcdecl to symbol entry
    header->sem();
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
}