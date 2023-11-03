#ifndef __LEXER_HPP__
#define __LEXER_HPP__

int yylex();
char Const_Char(const char* seq);
void yyerror(const char *msg);

#endif