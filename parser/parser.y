%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ast.h"  

void yyerror(const char *s);
int yylex();

%}

/* Define tokens */
%token <str> VARIABLE        
%token <str> TIME_VARIABLE   
%token <num> NUMBER          
%token BEGIN_EQUATION END_EQUATION SUM MINUS MULT DIV EXP EQUALSIGN
%token L_BRK R_BRK L_CB R_CB SUBINDEX FRAQ INF

/* Define datatypes for AST nodes */
%union {
    char* str;
    double num;
}

/* Define relation between tokens and types*/
%type <str> expression
%type <str> math_expression
%type <str> time_variable
%type <str> variable

/* Operators and precedence */
%left SUM MINUS
%left MULT DIV
%right EXP

%%

/* Grammar rules */
input:
    equations
    ;

equations:
    equation
    | equations equation
    ;

equation:
    BEGIN_EQUATION expression END_EQUATION {
        // printf("Equation detected: %s\n", $2);
    }
    ;

expression:
    variable EQUALSIGN math_expression  {
        if (eq_count < MAX_EQUATIONS && $1 != NULL && $3 != NULL) {
            // printf("Assigning: %s = %s\n", $1, $3); 
            strcpy(equations[eq_count].variable, $1);
            snprintf(equations[eq_count].equation, sizeof(equations[eq_count].equation), "%s", $3);
            eq_count++;
        } else {
            fprintf(stderr, "Error: Null pointer or equation limit exceeded\n");
        }
    }
    ;

math_expression:
    math_expression SUM math_expression  {
        asprintf(&$$, "%s + %s", $1, $3);
        // printf("Sum: %s\n", $$);
    }
    | math_expression MINUS math_expression  {
        asprintf(&$$, "%s - %s", $1, $3);
        // printf("Rest: %s\n", $$);
    }
    | math_expression MULT math_expression  {
        asprintf(&$$, "%s * %s", $1, $3);
        // printf("Multiplication: %s\n", $$);
    }
    | math_expression DIV math_expression  {
        asprintf(&$$, "%s / %s", $1, $3);
        // printf("Division: %s\n", $$);
    }
    | math_expression EXP math_expression  {
        asprintf(&$$, "%s^%s", $1, $3);
        // printf("Exponential: %s\n", $$);
    }
    | variable{
        $$ = strdup($1); 
        // printf("variable: %s\n", $$);
    }
    | L_BRK math_expression R_BRK  {
        asprintf(&$$, "(%s)", $2); 
        // printf("Brackets: %s\n", $$);
    }
    ;

variable:
    time_variable {
        $$ = strdup($1); 
        // printf("Time Variable: %s\n", $$);
    }
    | VARIABLE SUBINDEX L_CB INF R_CB {  
        asprintf(&$$, "%s_inf", $1); 
        // printf("Subindex with infinity: %s\n", $$);
        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = strdup($$);  // Agrega al array
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
    }
    | VARIABLE SUBINDEX L_CB VARIABLE R_CB {  
        asprintf(&$$, "%s_%s", $1, $4); 
        // printf("Subindex: %s\n", $$);
        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = strdup($$);  // Agrega al array
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
    }
    | VARIABLE  {
        $$ = strdup($1); 
        // printf("Variable: %s\n", $$);
        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = strdup($$);  // Agrega al array
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
    }
    | NUMBER  {
        asprintf(&$$, "%f", $1); 
        // printf("Number: %s\n", $$);
    }
    ;
time_variable:
    FRAQ L_CB TIME_VARIABLE R_CB L_CB TIME_VARIABLE R_CB {
        if ($3 != NULL) {
            // printf("Time Variable: %s\n", $3);
            if (n_variables < MAX_VARIABLES) {
                variables[n_variables++] = strdup($3);  // Agrega al array
            } else {
                fprintf(stderr, "Error: Time variables array is full.\n");
            }
            $$ = $3; // Propaga el valor hacia arriba
        } else {
            fprintf(stderr, "Error: Null pointer in time_variable.\n");
            $$ = NULL; // Maneja el error
        }
    }
    ;

%%
void print_variables() {
    // printf("Time Variables:\n");
    for (int i = 0; i < n_variables; i++) {
        printf("  %s\n", variables[i]);
    }

    // printf("Regular Variables:\n");
    for (int i = 0; i < n_parameters; i++) {
        printf("  %s\n", parameters[i]);
    }
}
/* Error function handler */
void yyerror(const char *s) {
    fprintf(stderr, "Sintax error: %s\n", s);
}

int yydebug = 1; // Debugging
int main() {
    // printf("Please give me model equations in LaTeX:\n");
    yyparse();
    generate_code();  // Generate code from the parsed equations.
    return 0;
}