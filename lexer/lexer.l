%{
#define T_eof        0
#define T_and        1000
#define T_char       1001
#define T_div        1002
#define T_do         1003
#define T_else       1004
#define T_fun        1005
#define T_if         1006
#define T_int        1007
#define T_mod        1008
#define T_not        1009
#define T_nothing    1010
#define T_or         1011
#define T_ref        1012
#define T_return     1013
#define T_then       1014
#define T_var        1015
#define T_while      1016

#define T_id         1017
#define T_const      1018
#define T_const_char 1019
#define T_const_str  1020
#define T_ge         1021
#define T_le         1022
#define T_assign     1023

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

%option nounput
%option noyywrap

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

int main() {
    int token;
    do {
        token = yylex();
        printf("token = %d, lexeme = \"%s\"\n", token, yytext);
    } while(token != T_eof);
    return 0;
}