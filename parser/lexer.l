%{
#include <stdio.h>
#include "parser.tab.h"
%}

%%

"\\begin{equation}"  { return BEGIN_EQUATION; }
"\\end{equation}"    { return END_EQUATION; }
"="                  { return EQUALSIGN; }
"+"                  { return SUM; }
"-"                  { return MINUS; }
"*"                  { return MULT; }
"/"                  { return DIV; }
"^"                  { return EXP; }
"("                  { return L_BRK; }
")"                  { return R_BRK; }
"{"                  { return L_CB; }
"}"                  { return R_CB; }
"_"                  { return SUBINDEX; }
"\\frac"             { return FRAQ; }
"\{\\infty\}"            { return INF; }

d([a-zA-Z][a-zA-Z]*)  {
    yylval.str = strdup(yytext + 1);
    if (yylval.str == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    return TIME_VARIABLE;
}

[a-zA-Z]([a-zA-Z]*|\{[a-zA-Z]+\})* {
    yylval.str = strdup(yytext);
    if (yylval.str == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    return VARIABLE;
}

[0-9]+(\.[0-9]+)?  { yylval.num = atof(yytext); return NUMBER; }


%.*  ;  // Ignore comments

\\label\{[a-zA-Z0-9_]+:[a-zA-Z0-9_]+\}  ;

[ \t\n]  ;  // Ignore white spaces \n

%%

int yywrap() { return 1; }