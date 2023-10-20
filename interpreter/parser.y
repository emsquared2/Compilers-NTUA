%{
    #include <cstdio>
    #include <stdio.h>
    #include <vector>
    #include "lexer.hpp"

    #define YYDEBUG 1

%}


%token T_and        "and"
%token T_bool       "bool"
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

         

%expect 1
%define parse.error detailed

%union {
    Expr *expr;
    Block *block;
    ExprList *exprlist;
    Cond *cond;
    Stmt *stmt;
    Decl *decl;
    FParam *fparam;
    Header *header;
    FuncDecl *funcdecl;
    FuncDef *funcdef;
    LocalDef *localdef;
    IdList *idlist;
    ParamList *paramlist;
    ArrayDim *arraydim;
    Type type; // as we deal with more  types might change 
    DataType *datatype;
    RetType *rettype;
    FParType *fpartype;
    Id *id;
    Const *cons;
    ConstChar *constchar;
    ConstStr *conststr;
    std::string op;
}

%token<id> T_id
%token<cons> T_const      
%token<constchar> T_const_char 
%token<conststr> T_const_str  
%token<op> T_ge         ">="     
%token<op> T_le         "<="
%token<op> T_assign     "<-"

%left<op> T_or
%left<op> T_and
%nonassoc<op> T_not
%nonassoc<op> "=" "#" ">" "<" T_ge T_le
%left "+" "-"
%left "*" T_div T_mod

%type<block> stmt-list block program
%type<exprlist> expr-list
%type<expr> expr l-value func-call
%type<decl> var-def
%type<cond> cond
%type<stmt> stmt
%type<fparam> fpar-def
%type<header> header
%type<funcdecl> func-decl
%type<funcdef> func-def
%type<localdef> local-def
%type<idlist> id-extended
%type<paramlist> fpar-def-extended
%type<arraydim> bracket-extended
%type<type> type;
%type<datatype> data-type
%type<rettype> ret-type
%type<fpartype> fpar-type


%%

program:
    /* nothing */   { std:cout << "Empty program" << std::endl; }
    | func-def  {  
        std:cout << "AST" << *$1 << std::endl;
        //$$ = $1; 
    }
;

func-def:
    header local-def block      { $$ = new FuncDef($1, $2, $3); }
;

header: 
    "fun" T_id '(' fpar-def fpar-def-extended ')' ':' ret-type  { $5->append($4); $$ = new Header($2, $8, $5); }
    | "fun" T_id '(' ')' ':' ret-type                           { $$ = new Header($2, $6); }
;

fpar-def-extended:
    /* nothing */                       { $$ = new ParamList();}
    | ';' fpar-def fpar-def-extended    { $3->append($2); $$ = $3;}
;

fpar-def:
    "ref" T_id id-extended ':' fpar-type { $3->append($2); $$ = new FParam($3, $5, true);}
    | T_id id-extended ':' fpar-type     { $2->append($1); $$ = new FParam($2, $4);}
;

id-extended:
    /* nothing */               { $$ = new IdList(); }
    | ',' T_id id-extended      { $3->append($2); $$ = $3; }
;

fpar-type: // use $2->isEmpty() to identify simple params (int / char) from arrays
    data-type bracket-extended              { $$ = new FParType($1, $2); }
    | data-type '[' ']' bracket-extended    { $$ = new FParType($1, $2, true); }
;

bracket-extended:
    /* nothing */                       { $$ = new ArrayDim(); } // use
    | '[' T_const ']' bracket-extended  { $4->append($2); $$ = $4;}
; 

data-type:
    "int"           /* { $$ = TYPE_INT;  } */ { $$ = new DataType($1);}
    | "char"        /* { $$ = TYPE_BOOL; } */ { $$ = new DataType($1);}
;

ret-type:
    data-type       { $$ = new RetType($1); }
    | "nothing"     { $$ = new RetType();   }
;

local-def:
    /* nothing*/                { $$ = new LocalDef(); }
    | func-def local-def        { $2->append($1); $$ = $2; }
    | func-decl local-def       { $2->append($1); $$ = $2; }
    | var-def local-def         { $2->append($1); $$ = $2; }
;

func-decl:
    header ';'  { $$ = new FuncDecl($1); }
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
    | T_id '(' expr expr-list ')'   { $4->append($3); $$ = new Call($1, $4); }
;

expr-list:
    /* nothing */           { $$ = new Expr_list();}
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