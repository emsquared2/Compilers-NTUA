#include "ast.hpp"

extern int yylineno;

AST::AST() : lineno(yylineno) {};

void AST::SemanticError(const char *msg)
{
    fprintf(stderr, "Error: %s at line %d\n", msg, lineno);
    // fprintf(stderr, "%s\n", msg);
    exit(1);
}