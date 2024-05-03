%{
    #include <cstdio>
    #include <cstdlib>

    #include "lexer.hpp"
    
    #include "ast.hpp"

    #include "array.hpp"
    #include "arraydim.hpp"
    #include "arrayelem.hpp"
    #include "assign.hpp"
    #include "binop.hpp"
    #include "block.hpp"
    #include "callexpr.hpp"
    #include "callstmt.hpp"
    #include "compcond.hpp"
    #include "cond.hpp"
    #include "const.hpp"
    #include "constchar.hpp"
    #include "conststr.hpp"
    #include "customtype.hpp"
    #include "decl.hpp"
    #include "expr.hpp"
    #include "exprlist.hpp"
    #include "fparam.hpp"
    #include "fparamlist.hpp"
    #include "fpartype.hpp"
    #include "funcdecl.hpp"
    #include "funcdef.hpp"
    #include "header.hpp"
    #include "id.hpp"
    #include "idlist.hpp"
    #include "if.hpp"
    #include "localdef.hpp"
    #include "localdeflist.hpp"
    #include "lvalue.hpp"
    #include "nothing.hpp"
    #include "opcond.hpp"
    #include "rettype.hpp"
    #include "return.hpp"
    #include "stmt.hpp"
    #include "stmtlist.hpp"
    #include "unop.hpp"
    #include "while.hpp"

    #define YYDEBUG 1

%}

%expect 1
%define parse.error /*detailed*/ verbose
%verbose
%define parse.trace

%union {

    Expr *expr;
    LValue *lvalue;
    Block *block;
    ExprList *exprlist;
    StmtList *stmtlist;
    Cond *cond;
    Stmt *stmt;
    Decl *decl;
    FParam *fparam;
    Header *header;
    LocalDef *localdef;
    FuncDecl *funcdecl;
    FuncDef *funcdef;
    LocalDefList *localdeflist;
    IdList *idlist;
    ArrayDim *arraydim;
    FParamList *fparamlist;

    CustomType *type;
    DataType data_type;

    RetType *ret_type;
    FParType *fpar_type;
    Id *id;
    Const *cons;
    ConstChar *constchar;
    ConstStr *conststr;
    char *op, c, *str;
    int num; 

    // CallStmt *callstmt;
    // CallExpr *callexpr;
}

%token T_and        "and"
%token T_bool       "bool"
%token<str> T_char  "char"
%token T_div        "div"
%token T_do         "do"
%token T_else       "else"
%token T_fun        "fun"
%token T_if         "if"
%token<str> T_int   "int"
%token T_mod        "mod"
%token T_not        "not"
%token T_nothing    "nothing"
%token T_or         "or"
%token T_ref        "ref"
%token T_return     "return"
%token T_then       "then"
%token T_var        "var"
%token T_while      "while"

%token T_ge         ">="     
%token T_le         "<="
%token<op> T_assign "<-"

//%token<id> T_id
%token<str> T_id
//%token<cons> T_const
%token<num> T_const      
%token<c> T_const_char 
%token<str> T_const_str 
/* %token<constchar> T_const_char 
%token<conststr> T_const_str   */


%left<op> T_or
%left<op> T_and
%nonassoc<op> T_not
%nonassoc<op> '=' '#' '>' '<' T_ge T_le
%left<op> '+' '-'
%left<op> '*' T_div T_mod


%type<block> block stmt_list
%type<funcdef> func_def program
%type<exprlist> expr_list array_elem_l_value

%type<expr> expr func_call_expr                 /* l_value*/ /* func_call */
%type<lvalue> l_value l_value_helper
%type<decl> var_def
%type<cond> cond
%type<stmt> stmt func_call_stmt                 /* func_call */ 
%type<fparam> fpar_def
%type<header> header
%type<localdef> local_def
%type<funcdecl> func_decl
%type<localdeflist> local_def_list
%type<idlist> id_list
%type<fparamlist> fpar_def_list
%type<arraydim> bracket_extended
%type<data_type> data_type
// %type<type> type 
%type<ret_type> ret_type
%type<fpar_type> fpar_type type
%type<cons> const
%type<id> id;

// %type<callstmt> func_call_stmt
// %type<callexpr> func_call_expr



%%

// Maybe implement class Program
program:
    /* nothing */   { std::cout << "Empty program" << std::endl; }
    | func_def      { std::cout << "AST: " << *$1 << std::endl; 
        $$ = $1; 
        $1->sem();
        delete $$;
    }
;

func_def:
    header local_def_list block      { $$ = new FuncDef($1, $2, $3); }
;

header: 
    "fun" id '(' fpar_def fpar_def_list ')' ':' ret_type  { $5->append($4); $$ = new Header($2, $8, $5); }
    | "fun" id '(' ')' ':' ret_type                       { $$ = new Header($2, $6, nullptr); }
;

fpar_def_list:
    /* nothing */                       { $$ = new FParamList();}
    | ';' fpar_def fpar_def_list        { $3->append($2); $$ = $3;}
;

fpar_def:
    "ref" id id_list ':' fpar_type { $3->append($2); $$ = new FParam($3, $5, true);}
    | id id_list ':' fpar_type     { $2->append($1); $$ = new FParam($2, $4, false);}
;

id_list:
    /* nothing */               { $$ = new IdList(); }
    | ',' id id_list            { $3->append($2); $$ = $3; }
;

fpar_type:
    data_type bracket_extended
    { 
        if($2->isEmpty()) {
            $$ = new FParType($1);
        }
        else {
            $$ = new FParType(new Array($1, $2));
        }
    }
    | data_type '[' ']' bracket_extended    { $4->setunknownFirstDim();  $$ = new FParType(new Array($1, $4)); }
; // array 

bracket_extended: 
    /* nothing */                                  { $$ = new ArrayDim();      }
    | '[' /* T_const*/ const ']' bracket_extended  { $4->append($2); $$ = $4;  }
; 

data_type:
    "int"           { $$ = TYPE_INTEGER; } 
    | "char"        { $$ = TYPE_CHAR; }
;

ret_type: 
    data_type       { $$ = new RetType($1);        }
    | "nothing"     { $$ = new RetType(TYPE_VOID); }
;

local_def_list:
    /* nothing*/                    { $$ = new LocalDefList(); }
    | local_def local_def_list      { $2->append($1); $$ = $2; }
;

local_def:
    func_def                    { $$ = $1; }
    | func_decl                 { $$ = $1; }
    | var_def                   { $$ = $1; }
;

func_decl:
    header ';'  { $$ = new FuncDecl($1); }
;

var_def:
    "var" id id_list ':' type ';'     { $3->append($2); $$ = new Decl($3, $5); }
;

type:
    data_type bracket_extended      
    { 
        if($2->isEmpty()) {
            $$ = new FParType($1);
        }
        else {
            $$ = new FParType(new Array($1, $2));
        } 
    }
;

block:
    '{' stmt_list '}'   { $$ = $2; }
;

stmt_list: 
    /* nothing */       { $$ = new Block(); }
    | stmt stmt_list    { if($1 != nullptr) { $2->append($1); } $$ = $2; }
;

stmt:
    ';'                                     { $$ = nullptr; }
    | l_value T_assign expr ';'             { $$ = new Assign($1, $3); }
    | block                                 { $$ = $1; }
    | func_call_stmt ';'                    { $$ = $1; }
    | "if" cond "then" stmt                 { $$ = new If($2, $4, nullptr); }
    | "if" cond "then" stmt "else" stmt     { $$ = new If($2, $4, $6); }
    | "while" cond "do" stmt                { $$ = new While($2, $4); }
    | "return" ';'                          { $$ = new Return(); }
    | "return" expr ';'                     { $$ = new Return($2); }    
;

// Maybe refactor implementation
l_value: 
    l_value_helper array_elem_l_value     
    { 
        if($2->isEmpty()) {
            $$ = $1;
        }
        else {
            $$ = new ArrayElem($1, $2);
        }
    }
;

l_value_helper:
    id                      { $$ = $1; }
    | T_const_str           { $$ = new ConstStr($1);}
;

array_elem_l_value:
    /* nothing */                               { $$ = new ExprList(); }
    | '[' expr ']' array_elem_l_value           { $4->append($2); $$ = $4; }
;


id :
    T_id                    { $$ = new Id($1);}
;

expr:
    // T_const             { $$ = new Const($1); }
    const
    | T_const_char      { $$ = new ConstChar($1); }
    | l_value           { $$ = $1; }
    | '(' expr ')'      { $$ = $2; }
    | func_call_expr    { $$ = $1; } // Check if run function needed.  
    | '+' expr          { $$ = new UnOp("+", $2); }
    | '-' expr          { $$ = new UnOp("-", $2); }
    | expr '+' expr     { $$ = new BinOp($1, "+", $3);}
    | expr '-' expr     { $$ = new BinOp($1, "-", $3);}
    | expr '*' expr     { $$ = new BinOp($1, "*", $3);}
    | expr "div"  expr  { $$ = new BinOp($1, "div", $3);}
    | expr "mod"  expr  { $$ = new BinOp($1, "mod", $3);}
;

const: 
    T_const           { $$ = new Const($1); }
;
// changed impl just to have correct type (custom Const class instead of int)
// Might need further examination ## 

func_call_stmt: 
    id '(' ')'                    { $$ = new CallStmt($1, nullptr); }
    | id '(' expr expr_list ')'   { $4->append($3); $$ = new CallStmt($1, $4); }
;

func_call_expr: 
    id '(' ')'                    { $$ = new CallExpr($1, nullptr); }
    | id '(' expr expr_list ')'   { $4->append($3); $$ = new CallExpr($1, $4); }
;

expr_list:
    /* nothing */           { $$ = new ExprList();}
    | ',' expr expr_list    { $3->append($2); $$ = $3;}
;

cond: 
    '(' cond ')'        { $$ = $2; }
    | "not" cond        { $$ = new OpCond("not", $2); }
    | cond "and" cond   { $$ = new OpCond($1, "and", $3); }       
    | cond "or" cond    { $$ = new OpCond($1, "or", $3); }    
    | expr '=' expr     { $$ = new CompCond($1, "=", $3);}  
    | expr '#' expr     { $$ = new CompCond($1, "#", $3);}
    | expr '<' expr     { $$ = new CompCond($1, "<", $3);}
    | expr '>' expr     { $$ = new CompCond($1, ">", $3);}
    | expr T_le expr    { $$ = new CompCond($1, "<=", $3);}
    | expr T_ge expr    { $$ = new CompCond($1, ">=", $3);}
;

%%

int main() {

    // Initialize the symbol table with hash table of size 1024
    initSymbolTable(1024);

    // Open program's scope
    openScope();

    #ifdef YYDEBUG
        int yydebug = 1;
    #endif

    int result = yyparse();
    if (result == 0) printf("Success.\n");
    return result;

    // Close program's scope
    closeScope();

    // Destroy the symbol table.
    destroySymbolTable();

}