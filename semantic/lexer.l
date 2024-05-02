%option nounput
%option noyywrap
%option yylineno

%{
#include <cstdio>
#include <cstdlib>
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
"bool"      { return T_bool; }
"char"      { yylval.str = strdup(yytext); return T_char; }
"div"       { return T_div; }
"do"        { return T_do; }
"else"      { return T_else; }
"fun"       { return T_fun; }
"if"        { return T_if; }
"int"       { yylval.str = strdup(yytext); return T_int; }
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


{L}({L}|{D}|\_)* { yylval.str = strdup(yytext); return T_id; }

{D}+ { yylval.num = std::stoi(yytext); return T_const; }

\'([^\'\"\\]|{Esc})\' { yylval.c = Const_Char(yytext); return T_const_char; }

\"([^\'\"\\]|{Esc})*\" { yylval.str = strdup(yytext); return T_const_str; }

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

char Const_Char(const char* seq) {
    if (seq[1] != '\\') {
        return seq[1];
    } else if (seq[2] == 'x' && isxdigit(seq[3]) && isxdigit(seq[4])) {
        char hexStr[3] = { seq[3], seq[4], '\0' };
        int asciiCode = strtol(hexStr, nullptr, 16);
        return static_cast<char>(asciiCode);
    } else {
        switch (seq[2]) {
            case 'n': return '\n';
            case 't': return '\t';
            case 'r': return '\r';
            case '0': return '\0';
            case '\\': return '\\';
            case '\'': return '\'';
            case '\"': return '\"';
            default:
                yyerror("Unrecognized const char!");
                return '\0'; // This will never be reached
        }
    }
}

void yyerror(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}