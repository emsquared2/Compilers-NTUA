#ifndef __LEXER_HPP__
#define __LEXER_HPP__

int yylex();
char Const_Char(const char* seq);
char *Const_String(const char * yytext);
void yyerror(const char *msg);

#endif