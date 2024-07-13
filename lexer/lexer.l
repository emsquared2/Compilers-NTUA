%{
    #include <string>
    #include <iostream>
        
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

    void show_help(const char *program_name) {
        std::cout << "Usage: " << program_name << " [options] <input_file>\n"
                << "Options:\n"
                << "  -l             Print lexical tokens.\n"
                << "  -h             Show this help message\n";
    }

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

int main(int argc, char *argv[]) {
    int opt;

    bool printTokens = false;
    std::string filename;

    // Process command line options
    while ((opt = getopt(argc, argv, "hl")) != -1) {
        switch (opt) {
            case 'l':
                printTokens = true; // Print the Abstract Syntax Tree (AST)
                break;
            case 'h':
                show_help(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                show_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Determine the input source based on command line arguments
    FILE* fp;
    if (optind < argc) {
        // A file is specified on the command line
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            fprintf(stderr, "Error opening file %s\n", argv[optind]);
            exit(EXIT_FAILURE);
        }
    } else {
        // If no file is specified, also print usage information and exit
        show_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin = fp;  // Set the input file for the lexer
    filename = argv[optind];  // This will always be valid as we exit if no file is specified


    int token;
    if (printTokens)
        do {
            token = yylex();
            printf("token = %d, lexeme = \"%s\"\n", token, yytext);
        } while(token != T_eof);
    printf("Success.\n");
    return 0;
}