%{
    #include <cstdio>
    #include <stdio.h>
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

%%

program:
    /* nothing */ 
    | func-def 
;

func-def:
    header local-def block 
;

header: 
    "fun" T_id '(' fpar-def fpar-def-extended ')' ':' ret-type
    | "fun" T_id '(' ')' ':' ret-type
;

fpar-def-extended:
    /* nothing */
    | ';' fpar-def fpar-def-extended
;

fpar-def:
    "ref" T_id id-extended ':' fpar-type
    | T_id id-extended ':' fpar-type
;

id-extended:
    /* nothing */
    | ',' T_id id-extended
;

fpar-type:
    data-type bracket-extended
    | data-type '[' ']' bracket-extended
;

bracket-extended:
    /* nothing */
    | '[' T_const ']' bracket-extended
; 

data-type:
    "int"
    | "char"
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
    "var" T_id id-extended ':' type ';'
;

type:
    data-type bracket-extended
;

block:
    '{' stmt-list '}'
;

stmt-list:
    /* nothing */
    | stmt stmt-list
;

stmt:
    ';'
    | l-value T_assign expr ';' 
    | block
    | func-call ';'
    | "if" cond "then" stmt 
    | "if" cond "then" stmt "else" stmt
    | "while" cond "do" stmt 
    | "return" ';'
    | "return" expr ';'
;

l-value:
    T_id 
    | T_const_str
    | l-value '[' expr ']'
;

expr:
    T_const
    | T_const_char
    | l-value
    | '(' expr ')'
    | func-call
    | '+' expr
    | '-' expr
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr "div"  expr
    | expr "mod"  expr
;

func-call: 
    T_id '(' ')'
    | T_id '(' expr expr-list ')'
;

expr-list:
    /* nothing */
    | ',' expr expr-list
;

cond: 
    '(' cond ')'
    | "not" cond
    | cond "and" cond
    | cond "or" cond
    | expr '=' expr
    | expr '#' expr
    | expr '<' expr
    | expr '>' expr
    | expr T_le expr
    | expr T_ge expr
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