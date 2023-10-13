%option nounput
%option noyywrap

%{
#include <cstdio>
#include <cstdlib>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int lineno = 1;
%}

/* letters */
L [a-zA-Z]

/* digits */
D [0-9]

/* whitespaces */
W [ \t\r]

/* hex digits */
HEX [0-9a-fA-F]

/* escape sequences */
Esc \\([ntr0\\\'\"]|x{HEX}{HEX})

%x COMMENT


%%

"and"       { return T_and; }
"char"      { return T_char; }
"div"       { return T_div; }
"do"        { return T_do; }
"else"      { return T_else; }
"fun"       { return T_fun; }
"if"        { return T_if; }
"int"       { return T_int; }
"mod"       { return T_mod; }
"not"       { return T_not; }
"nothing"   { return T_nothing; }
"or"        { return T_or; }
"ref"       { return T_ref; }
"return"    { return T_return; }
"then"      { return T_then; }
"var"       { return T_var; }
"while"     { return T_while; }

">="        { return T_ge; }
"<="        { return T_le; }
"<-"        { return T_assign; }


{L}({L}|{D}|\_)* { return T_id; }

{D}+ { return T_const; }

\'([^\'\"\\]|{Esc})\' { return T_const_char; }

\"([^\'\"\\]|{Esc})*\" { return T_const_str; }

[\+\-\*\=\#\<\>] { return yytext[0]; }

[\(\)\[\]\{\}\,\;\:] { return yytext[0]; }

{W}+    { /* return nothing */ }

\n      { ++lineno; }

"$$"                 { BEGIN(COMMENT); }
<COMMENT>"$$"        { BEGIN(INITIAL); }
<COMMENT>\n          { lineno++; }
<COMMENT>"$"         { /* nothing */ }
<COMMENT>[^$\n]+     { /* nothing */ }
<COMMENT><<EOF>>     { fprintf(stderr, "Unclosed comment\n"); exit(42); }

$[^\$\n]*\n?         { lineno++; }

.       { fprintf(stderr, "Illegal character %c at line %d\n", yytext[0], lineno); exit(17); }

%%

void yyerror(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}