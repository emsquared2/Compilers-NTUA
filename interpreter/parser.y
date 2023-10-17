%{
    #include <cstdio>
    #include <stdio.h>
    #include <vector>
    #include "lexer.hpp"

    #define YYDEBUG 1

%}


%token T_and        "and"
%token T_char       "char"
%token T_div        "div"
%token T_do         "do"
%token T_else       "else"
%token T_fun        "fun"
%token T_if         "if"
%token T_int        "int"
%token T_mod        "mod"
%token T_not        "not"
%token T_nothing    "nothing"
%token T_or         "or"
%token T_ref        "ref"
%token T_return     "return"
%token T_then       "then"
%token T_var        "var"
%token T_while      "while"

%token T_id         
%token T_const      
%token T_const_char 
%token T_const_str  
%token T_ge         ">="     
%token T_le         "<="
%token T_assign     "<-"

%left T_or
%left T_and
%nonassoc T_not
%nonassoc '=' '#' '>' '<' T_ge T_le
%left '+' '-'
%left '*' T_div T_mod

%expect 1
%define parse.error detailed

%union {
    Expr *expr;
    Block *block;
    Cond *cond;
    Stmt *stmt;
    Decl *decl;
    IdList *idlist;
    ParamList *paramlist;
    ArrayDim *arraydim;
    Type type; // as we deal with more  types might change 
}

%type<block> stmt-list, expr-list, block, program
%type<expr> expr, l-value, func-call
%type<decl> var-def
%type<cond> cond
%type<stmt> stmt
%type<idlist> id-extended
%type<paramlist> fpar-def-extended
%type<arraydim> bracket-extended
%type<type> data-type;

%%

program:
    /* nothing */ 
    | func-def  {  
        std:cout << "AST" << *$1 << std::endl;
        //$$ = $1; 
    }
;

func-def:
    header local-def block 
;

header: 
    "fun" T_id '(' fpar-def fpar-def-extended ')' ':' ret-type
    | "fun" T_id '(' ')' ':' ret-type
;

fpar-def-extended:
    /* nothing */                       { $$ = new ParamList();}
    | ';' fpar-def fpar-def-extended    { $3->append($2); $$ = $3;}
;

fpar-def:
    "ref" T_id id-extended ':' fpar-type
    | T_id id-extended ':' fpar-type
;

id-extended:
    /* nothing */               { $$ = new IdList(); }
    | ',' T_id id-extended      { $3->append($2); $$ = $3; }
;

fpar-type:
    data-type bracket-extended
    | data-type '[' ']' bracket-extended
;

bracket-extended:
    /* nothing */                       { $$ = new ArrayDim(); }
    | '[' T_const ']' bracket-extended  { $4->append($2); $$ = $4;}
; 

data-type:
    "int"           { $$ = TYPE_INT;  }
    | "char"        { $$ = TYPE_BOOL; }
;

ret-type:
    data-type
    | "nothing"
;

local-def:
    /* nothing*/
    | func-def local-def
    | func-decl local-def
    | var-def local-def
;

func-decl:
    header ';'
;

var-def:
    "var" T_id id-extended ':' type ';'     { $3->append($2); $$ = new Decl($3, $5); }
;

type:
    data-type bracket-extended      { $$ = new Type($1, $2); }
;

block:
    '{' stmt-list '}'   { $$ = $2; }
;

stmt-list:
    /* nothing */       { $$ = new Block(); }
    | stmt stmt-list    { $2->append($1); $$ = $2;}
;

stmt:
    ';'
    | l-value T_assign expr ';'             { $$ = new Assign($1, $3); }
    | block                                 { $$ = $1; }
    | func-call ';'                         { $$ = $1; }
    | "if" cond "then" stmt                 { $$ = new If($2, $4); }
    | "if" cond "then" stmt "else" stmt     { $$ = new If($2, $4, $6); }
    | "while" cond "do" stmt                { $$ = new While($2, $4); }
    | "return" ';'                          { $$ = new Return(); }
    | "return" expr ';'                     { $$ = new Return($2); }    
;

l-value:                   
    T_id                    { $$ = new Id($1);}
    | T_const_str           { $$ = new ConstStr($1);}
    | l-value '[' expr ']'  { $$ = new ArrayElem($1, $3);} 
;

expr:
    T_const             { $$ = new Const($1); }
    | T_const_char      { $$ = new ConstChar($1); }
    | l-value           { $$ = $1; }
    | '(' expr ')'      { $$ = $2; }
    | func-call         { $$ = $1; } // Check if run function needed.  
    | '+' expr          { $$ = new Unop(std::string($1), $2); }
    | '-' expr          { $$ = new Unop(std::string($1), $2); }
    | expr '+' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr '-' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr '*' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr "div"  expr  { $$ = new BinOp($1, $2, $3);}
    | expr "mod"  expr  { $$ = new BinOp($1, $2, $3);}
;

func-call: 
    T_id '(' ')'                    { $$ = new Call($1); }
    | T_id '(' expr expr-list ')'   { $$ = new Call($1, $3, $4); }
;

expr-list:
    /* nothing */           { $$ = new Block();}
    | ',' expr expr-list    { $3->append($2); $$ = $3;}
;

cond: 
    '(' cond ')'        { $$ = $2; }
    | "not" cond        { $$ = new OpCond($1, $2); }
    | cond "and" cond   { $$ = new OpCond($1, $2, $3); }       
    | cond "or" cond    { $$ = new OpCond($1, $2, $3); }    
    | expr '=' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr '#' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr '<' expr     { $$ = new BinOp($1, std::string($2), $3);}
    | expr '>' expr     { $$ = new BinOp($1, std::string($2), $3);}
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