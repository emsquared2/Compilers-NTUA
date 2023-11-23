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
    ArrayDim *arraydim;
    ParamList *paramlist;
    Type *type /**rettype, *fpartype*/; // as we deal with more types might change 
    // DataType *datatype;
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
%type<exprlist> expr-list array-elem-l-value
%type<expr> expr func-call-expr/*l-value*/ /*func-call*/
%type<lvalue> l-value l-value-helper
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
%type<type> type data-type //ret-type fpar-type
/* %type<datatype> data-type */
%type<rettype> ret-type
%type<fpartype> fpar-type
%type<cons> const
%type<id> id;


%%

program:
    /* nothing */   { std::cout << "Empty program" << std::endl; }
    | func-def  {  std::cout << "AST: " << *$1 << std::endl;
        //$$ = $1; 
    }
;

func-def:
    header local-def block      { $$ = new FuncDef($1, $2, $3); }
;

header: 
    "fun" id '(' fpar-def fpar-def-extended ')' ':' ret-type  { $5->append($4); $$ = new Header($2, $8, $5); }
    | "fun" id '(' ')' ':' ret-type                           { $$ = new Header($2, $6); }
;

fpar-def-extended:
    /* nothing */                       { $$ = new ParamList();}
    | ';' fpar-def fpar-def-extended    { $3->append($2); $$ = $3;}
;

fpar-def:
    "ref" id id-extended ':' fpar-type { $3->append($2); $$ = new FParam($3, $5, true);}
    | id id-extended ':' fpar-type     { $2->append($1); $$ = new FParam($2, $4);}
;

id-extended:
    /* nothing */               { $$ = new IdList(); }
    | ',' id id-extended        { $3->append($2); $$ = $3; }
;

fpar-type: // use $2->isEmpty() to identify simple params (int / char) from arrays
    data-type bracket-extended
    { 
        if($2->isEmpty()) {
            $$ = new FParType($1);
        }
        else {
            $$ = new FParType(new Array($1, $2));
        }
    }
    | data-type '[' ']' bracket-extended    { $4->specifyUnknownFirstDim();  $$ = new FParType(new Array($1, $4)); }
; // array 

bracket-extended: 
    /* nothing */                                 { $$ = new ArrayDim(); } // use
    | '[' /*T_const*/ const ']' bracket-extended  { $4->append($2); $$ = $4;}
; 

data-type:
    "int"           { $$ = new Integer(); } 
    | "char"        { $$ = new Character();}
;

ret-type: 
    data-type       { $$ = new RetType($1); }
    | "nothing"     { $$ = new RetType();   }
;

local-def:
    /* nothing*/                { $$ = new LocalDef(); }
    | local local-def           { $2->append($1); $$ = $2; }
    /* | func-def local-def        { $2->append($1); $$ = $2; }
    | func-decl local-def       { $2->append($1); $$ = $2; }
    | var-def local-def         { $2->append($1); $$ = $2; } */
;

local:
    func-def                    { $$ = new Local($1); }
    | func-decl                 { $$ = new Local($1); }
    | var-def                   { $$ = new Local($1); }
;

func-decl:
    header ';'  { $$ = new FuncDecl($1); }
;

var-def:
    "var" id id-extended ':' type ';'     { $3->append($2); $$ = new Decl($3, $5); }
;

type:
    data-type bracket-extended      
    { 
        if($2->isEmpty()) {
            $$ = $1;
        }
        else {
            $$ = new Array($1, $2);
        } 
    }
;

block:
    '{' stmt-list '}'   { $$ = $2; }
;

stmt-list: 
    /* nothing */       { $$ = new Block(); }
    | stmt stmt-list    { $2->append($1); $$ = $2;}
;

stmt:
    ';'                                     { $$ = nullptr; }
    | l-value T_assign expr ';'             { $$ = new Assign($1, $3); }
    | block                                 { $$ = $1; }
    | func-call-stmt ';'                    { $$ = $1; }
    | "if" cond "then" stmt                 { $$ = new If($2, $4); }
    | "if" cond "then" stmt "else" stmt     { $$ = new If($2, $4, $6); }
    | "while" cond "do" stmt                { $$ = new While($2, $4); }
    | "return" ';'                          { $$ = new Return(); }
    | "return" expr ';'                     { $$ = new Return($2); }    
;

l-value: 
    l-value-helper array-elem-l-value     
    { 
        if($2->isEmpty()) {
            $$ = $1;
        }
        else {
            $$ = new ArrayElem($1, $2);
        }
    }

l-value-helper:
    id                      { $$ = $1; }
    | T_const_str           { $$ = new ConstStr($1);}

array-elem-l-value:
    /* nothing */                               { $$ = new ExprList(); }
    | '[' expr ']' array-elem-l-value           { $4->append($2); $$ = $4; }


id :
    T_id                    { $$ = new Id($1);}
;
// changed all T_id to id to satisfy correct id

expr:
    //T_const             { $$ = new Const($1); }
    const               { $$ = $1; }
    | T_const_char      { $$ = new ConstChar($1); }
    | l-value           { $$ = $1; }
    | '(' expr ')'      { $$ = $2; }
    | func-call-expr    { $$ = $1; } // Check if run function needed.  
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

func-call-stmt: 
    id '(' ')'                    { $$ = new CallStmt($1); }
    | id '(' expr expr-list ')'   { $4->append($3); $$ = new CallStmt($1, $4); }
;

func-call-expr: 
    id '(' ')'                    { $$ = new CallExpr($1); }
    | id '(' expr expr-list ')'   { $4->append($3); $$ = new CallExpr($1, $4); }
;

expr-list:
    /* nothing */           { $$ = new ExprList();}
    | ',' expr expr-list    { $3->append($2); $$ = $3;}
;

cond: 
    '(' cond ')'        { $$ = $2; }
    | "not" cond        { $$ = new OpCond("not", $2); }
    | cond "and" cond   { $$ = new OpCond($1, "and", $3); }       
    | cond "or" cond    { $$ = new OpCond($1, "or", $3); }    
    | expr '=' expr     { $$ = new BinOp($1, "=", $3);}
    | expr '#' expr     { $$ = new BinOp($1, "#", $3);}
    | expr '<' expr     { $$ = new BinOp($1, "<", $3);}
    | expr '>' expr     { $$ = new BinOp($1, ">", $3);}
    | expr T_le expr    { $$ = new BinOp($1, "<=", $3);}
    | expr T_ge expr    { $$ = new BinOp($1, ">=", $3);}
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