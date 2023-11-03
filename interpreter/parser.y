%{
    #include <cstdio>
    #include <stdio.h>
    #include <vector>
    #include "lexer.hpp"
    #include "ast.hpp"
    #include "localdef.hpp"
    #include "funcdef.hpp"
    #include "local.hpp"

    #define YYDEBUG 1

%}


%token T_and        "and"
%token T_bool       "bool"
%token<str> T_char    "char"
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
    Local *local;
    FuncDecl *funcdecl;
    FuncDef *funcdef;
    LocalDef *localdef;
    IdList *idlist;
    ParamList *paramlist;
    ArrayDim *arraydim;
    Type *type; // as we deal with more  types might change 
    DataType *datatype;
    RetType *rettype;
    FParType *fpartype;
    Id *id;
    Const *cons;
    ConstChar *constchar;
    ConstStr *conststr;
    char *op, c, *str;
    //std::string str;
    int num;
}

//%token<id> T_id
%token<str> T_id
//%token<cons> T_const
%token<num> T_const      
%token<c> T_const_char 
%token<str> T_const_str 
/* %token<constchar> T_const_char 
%token<conststr> T_const_str   */
%token T_ge         ">="     
%token T_le         "<="
%token<op> T_assign     "<-"

%left<op> T_or
%left<op> T_and
%nonassoc<op> T_not
%nonassoc<op> '=' '#' '>' '<' T_ge T_le
%left<op> '+' '-'
%left<op> '*' T_div T_mod

%type<block> stmt-list block program
%type<funcdef> func-def /*program*/
%type<exprlist> expr-list
%type<expr> expr func-call-expr/*l-value*/ /*func-call*/
%type<lvalue> l-value
%type<decl> var-def
%type<cond> cond
//%type<stmtlist> stmt-list
%type<stmt> stmt /*func-call */ func-call-stmt
%type<fparam> fpar-def
%type<header> header
%type<local> local
%type<funcdecl> func-decl
%type<localdef> local-def
%type<idlist> id-extended
%type<paramlist> fpar-def-extended
%type<arraydim> bracket-extended
%type<type> type;
%type<datatype> data-type
%type<rettype> ret-type
%type<fpartype> fpar-type
%type<cons> const
%type<id> id;


%%

program:
    /* nothing */   { std::cout << "Empty program" << std::endl; }
    | func-def  {  std::cout << "AST" << *$1 << std::endl;
        //$$ = $1; 
    }
;

func-def:
    header local-def block      { std::cout << "here"<< std::endl; $$ = new FuncDef($1, $2, $3); }
;

header: 
    "fun" id '(' fpar-def fpar-def-extended ')' ':' ret-type  { std::cout << "here2"<< std::endl; $5->append($4); $$ = new Header($2, $8, $5); }
    | "fun" id '(' ')' ':' ret-type                           { std::cout << "here3"<< std::endl; $$ = new Header($2, $6); }
;

fpar-def-extended:
    /* nothing */                       { std::cout << "here4"<< std::endl; $$ = new ParamList();}
    | ';' fpar-def fpar-def-extended    { std::cout << "here5"<< std::endl; $3->append($2); $$ = $3;}
;

fpar-def:
    "ref" id id-extended ':' fpar-type { std::cout << "here6"<< std::endl; $3->append($2); $$ = new FParam($3, $5, true);}
    | id id-extended ':' fpar-type     { std::cout << "here7"<< std::endl; $2->append($1); $$ = new FParam($2, $4);}
;

id-extended:
    /* nothing */               {std::cout << "here8"<< std::endl; $$ = new IdList(); }
    | ',' id id-extended        {std::cout << "here9"<< std::endl; $3->append($2); $$ = $3; }
;

fpar-type: // use $2->isEmpty() to identify simple params (int / char) from arrays
    data-type bracket-extended              { std::cout << "here10"<< std::endl; $$ = new FParType($1, $2); }
    | data-type '[' ']' bracket-extended    { std::cout << "here11"<< std::endl; $$ = new FParType($1, $4, true); }
;

bracket-extended:
    /* nothing */                                 {std::cout << "here12"<< std::endl; $$ = new ArrayDim(); } // use
    | '[' /*T_const*/ const ']' bracket-extended  {std::cout << "here13"<< std::endl; $4->append($2); $$ = $4;}
; 

data-type:
    "int"           /* { $$ = TYPE_INT;  } */ { std::cout << "here14"<< std::endl; $$ = new DataType($1);}
    | "char"        /* { $$ = TYPE_BOOL; } */ { std::cout << "here15"<< std::endl; $$ = new DataType($1);}
;

ret-type:
    data-type       {std::cout << "here16"<< std::endl; $$ = new RetType($1); }
    | "nothing"     {std::cout << "here17"<< std::endl; $$ = new RetType();   }
;

local-def:
    /* nothing*/                {std::cout << "here18"<< std::endl; $$ = new LocalDef(); }
    | local local-def           {std::cout << "here19"<< std::endl; $2->append($1); $$ = $2; }
    /* | func-def local-def        {std::cout << "here19"<< std::endl; $2->append($1); $$ = $2; }
    | func-decl local-def       {std::cout << "here20"<< std::endl; $2->append($1); $$ = $2; }
    | var-def local-def         {std::cout << "here21"<< std::endl; $2->append($1); $$ = $2; } */
;

local:
    func-def                    {std::cout << "here20"<< std::endl; $$ = new Local($1); }
    | func-decl                 {std::cout << "here21"<< std::endl; $$ = new Local($1); }
    | var-def                   {std::cout << "here22"<< std::endl; $$ = new Local($1); }
;

func-decl:
    header ';'  {std::cout << "here22"<< std::endl; $$ = new FuncDecl($1); }
;

var-def:
    "var" id id-extended ':' type ';'     {std::cout << "here23"<< std::endl; $3->append($2); $$ = new Decl($3, $5); }
;

type:
    data-type bracket-extended      { std::cout << "here24"<< std::endl; $$ = new Type($1, $2); }
;

block:
    '{' stmt-list '}'   { std::cout << "here25"<< std::endl; $$ = $2; }
;

stmt-list: 
    /* nothing */       { std::cout << "here26"<< std::endl; $$ = new Block(); }
    | stmt stmt-list    { std::cout << "here27"<< std::endl; $2->append($1); $$ = $2;}
;

stmt:
    ';'                                     { std::cout << "here28"<< std::endl; $$ = nullptr; }
    | l-value T_assign expr ';'             { std::cout << "here29"<< std::endl; $$ = new Assign($1, $3); }
    | block                                 { std::cout << "here30"<< std::endl; $$ = $1; }
    | func-call-stmt ';'                    { std::cout << "here31"<< std::endl; $$ = $1; }
    | "if" cond "then" stmt                 { std::cout << "here32"<< std::endl; $$ = new If($2, $4); }
    | "if" cond "then" stmt "else" stmt     { std::cout << "here33"<< std::endl; $$ = new If($2, $4, $6); }
    | "while" cond "do" stmt                { std::cout << "here34"<< std::endl; $$ = new While($2, $4); }
    | "return" ';'                          { std::cout << "here35"<< std::endl; $$ = new Return(); }
    | "return" expr ';'                     { std::cout << "here36"<< std::endl; $$ = new Return($2); }    
;

l-value:                   
    id/*T_id*/              { std::cout << "here37"<< std::endl; $$ = $1;}
    | T_const_str           { std::cout << "here38"<< std::endl; $$ = new ConstStr($1);}
    | l-value '[' expr ']'  { std::cout << "here39"<< std::endl; $$ = new ArrayElem($1, $3);} 
;

id :
    T_id                    { std::cout << "here40" << $1 << std::endl;  $$ = new Id($1);}
;
// changed all T_id to id to satisfy correct id

expr:
    //T_const             { $$ = new Const($1); }
    const               { std::cout << "here41"<< std::endl; $$ = $1; }
    | T_const_char      { std::cout << "here42"<< std::endl; $$ = new ConstChar($1); }
    | l-value           { std::cout << "here43"<< std::endl; $$ = $1; }
    | '(' expr ')'      { std::cout << "here44"<< std::endl; $$ = $2; }
    | func-call-expr    { std::cout << "here45"<< std::endl; $$ = $1; } // Check if run function needed.  
    | '+' expr          { std::cout << "here46"<< std::endl; $$ = new UnOp("+", $2); }
    | '-' expr          { std::cout << "here47"<< std::endl; $$ = new UnOp("-", $2); }
    | expr '+' expr     { std::cout << "here48"<< std::endl; $$ = new BinOp($1, "+", $3);}
    | expr '-' expr     { std::cout << "here49"<< std::endl; $$ = new BinOp($1, "-", $3);}
    | expr '*' expr     { std::cout << "here50"<< std::endl; $$ = new BinOp($1, "*", $3);}
    | expr "div"  expr  { std::cout << "here51"<< std::endl; $$ = new BinOp($1, "div", $3);}
    | expr "mod"  expr  { std::cout << "here52"<< std::endl; $$ = new BinOp($1, "mod", $3);}
;

const: 
    T_const           { std::cout << "here43"<< std::endl; $$ = new Const($1); }
;
// changed impl just to have correct type (custom Const class instead of int)
// Might need further examination ## 

func-call-stmt: 
    id '(' ')'                    { std::cout << "here53"<< std::endl; $$ = new CallStmt($1); }
    | id '(' expr expr-list ')'   { std::cout << "here54"<< std::endl; $4->append($3); $$ = new CallStmt($1, $4); }
;

func-call-expr: 
    id '(' ')'                    {std::cout << "here55"<< std::endl; $$ = new CallExpr($1); }
    | id '(' expr expr-list ')'   {std::cout << "here56"<< std::endl; $4->append($3); $$ = new CallExpr($1, $4); }
;

expr-list:
    /* nothing */           { std::cout << "here57"<< std::endl; $$ = new ExprList();}
    | ',' expr expr-list    { std::cout << "here58"<< std::endl; $3->append($2); $$ = $3;}
;

cond: 
    '(' cond ')'        { std::cout << "here59"<< std::endl; $$ = $2; }
    | "not" cond        { std::cout << "here61"<< std::endl; $$ = new OpCond("not", $2); }
    | cond "and" cond   { std::cout << "here62"<< std::endl; $$ = new OpCond($1, "and", $3); }       
    | cond "or" cond    { std::cout << "here63"<< std::endl; $$ = new OpCond($1, "or", $3); }    
    | expr '=' expr     { std::cout << "here64"<< std::endl; $$ = new BinOp($1, "=", $3);}
    | expr '#' expr     { std::cout << "here65"<< std::endl; $$ = new BinOp($1, "#", $3);}
    | expr '<' expr     { std::cout << "here66"<< std::endl; $$ = new BinOp($1, "<", $3);}
    | expr '>' expr     { std::cout << "here67"<< std::endl; $$ = new BinOp($1, ">", $3);}
    | expr T_le expr    { std::cout << "here68"<< std::endl; $$ = new BinOp($1, "<=", $3);}
    | expr T_ge expr    { std::cout << "here69"<< std::endl; $$ = new BinOp($1, ">=", $3);}
;

%%

int main() {
    // #ifdef YYDEBUG
    //     int yydebug = 1;
    // #endif

    int result = yyparse();
    if (result == 0) printf("Success.\n");
    return result;
}