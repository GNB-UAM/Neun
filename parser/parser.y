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
%token BEGIN_EQUATION END_EQUATION SUM MINUS MULT DIV POW EXP EQUALSIGN 
%token L_BRK R_BRK L_CB R_CB SUBINDEX FRAQ INF COLON

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
%type <str> function

/* Operators and precedence */
%left SUM MINUS
%left MULT DIV
%right POW EXP

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
        if (DEBUG) printf("Equation detected: %s\n", $2);
    }
    ;

expression:
    variable EQUALSIGN math_expression  {
        if (eq_count < MAX_EQUATIONS && $1 != NULL && $3 != NULL) {
            if (DEBUG) printf("Assigning: %s = %s\n", $1, $3); 
            if(is_timevariable)
            {
                strcpy(incs_equations[incs_eq_count].variable, $1);
                snprintf(incs_equations[incs_eq_count].equation, sizeof(incs_equations[incs_eq_count].equation), "%s", $3);
                incs_eq_count++;
                is_timevariable = 0;
            }
            else if (is_function)
            {
                snprintf(functions[fun_count].equation, sizeof(functions[fun_count].equation), "%s", $3);
                fun_count++;
            }
            else
            {
                strcpy(equations[eq_count].variable, $1);
                snprintf(equations[eq_count].equation, sizeof(equations[eq_count].equation), "%s", $3);
                eq_count++;
            }

        } else {
            fprintf(stderr, "Error: Null pointer or equation limit exceeded\n");
        }
    }
    ;

math_expression:
    math_expression SUM math_expression  {
        asprintf(&$$, "%s + %s", $1, $3);
        if (DEBUG) printf("Sum: %s\n", $$);
    }
    | math_expression MINUS math_expression  {
        asprintf(&$$, "%s - %s", $1, $3);
        if (DEBUG) printf("Rest: %s\n", $$);
    }
    | math_expression MULT math_expression  {
        asprintf(&$$, "%s * %s", $1, $3);
        if (DEBUG) printf("Multiplication: %s\n", $$);
    }
    | math_expression DIV math_expression  {
        asprintf(&$$, "(%s) / (%s)", $1, $3);
        if (DEBUG) printf("Division: %s\n", $$);
    }
    | math_expression POW math_expression  {
        asprintf(&$$, "pow(%s, %s)", $1, $3);
        if (DEBUG) printf("Potential: %s\n", $$);
    }
    | EXP L_BRK math_expression R_BRK {
        asprintf(&$$, "exp(%s)", $3);
        if (DEBUG) printf("Exponential: %s\n", $$);
    }
    | FRAQ L_CB math_expression R_CB L_CB math_expression R_CB {
        asprintf(&$$, "(%s) / (%s)", $3, $6);
        if (DEBUG) printf("FRAQ: %s\n", $$);
    }
    | MINUS math_expression  {
        asprintf(&$$, "- %s", $2);
        if (DEBUG) printf("Rest: %s\n", $$);
    }
    | variable{
        $$ = strdup($1); 
        if (DEBUG) printf("variable: %s\n", $$);
    }
    | L_BRK math_expression R_BRK  {
        asprintf(&$$, "(%s)", $2); 
        if (DEBUG) printf("Brackets: %s\n", $$);
    }
    ;

variable:
    function{
        $$ = strdup($1); 
        if (DEBUG) printf("Function: %s\n", $$);
    }
    | time_variable {
        $$ = strdup($1); 
        is_timevariable = 1;
        if (DEBUG) printf("Time Variable: %s\n", $$);
    }
    | VARIABLE SUBINDEX L_CB INF R_CB {  
        asprintf(&$$, "%s_inf", $1); 
        if (DEBUG) printf("Subindex with infinity: %s\n", $$);
        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = strdup($$);  // Agrega al array
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
        // asprintf(&$$, "params[%s_inf]", $1); 
        // if (DEBUG) printf("Subindex with infinity: %s\n", $$);
    }
    | VARIABLE SUBINDEX L_CB VARIABLE R_CB {  
        asprintf(&$$, "%s_%s", $1, $4); 
        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = strdup($$);  // Agrega al array
        if (DEBUG) printf("Subindex: %s\n", $$);
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
        // asprintf(&$$, "params[%s_%s]", $1, $4); 
        // if (DEBUG) printf("Subindex: %s\n", $$);
    }
    | VARIABLE  {
        // $$ = strdup($1);
        
        asprintf(&$$, "%s", $1); 
        if (DEBUG) printf("Variable: %s\n", $$);

        if (n_parameters < MAX_VARIABLES) {
            parameters[n_parameters++] = $1;  // Agrega al array
        } else {
            fprintf(stderr, "Error: Regular variables array is full.\n");
        }
    }
    | NUMBER  {
        asprintf(&$$, "%f", $1); 
        if (DEBUG) printf("Number: %s\n", $$);
    }
    ;
time_variable:
    FRAQ L_CB TIME_VARIABLE R_CB L_CB TIME_VARIABLE R_CB {
        if ($3 != NULL) {
            if (DEBUG) printf("Time Variable: %s\n", $3);
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

function:
//TODO include several args
    variable L_BRK variable R_BRK
    {
        asprintf(&$$, "%s(%s)", $1, $3); 
        if (DEBUG) printf("Function: %s\n", $$);
        strcpy(functions[fun_count].name, $1);
        strcpy(functions[fun_count].args[0], $3);
        // fun_count++;
    };


%%
void print_variables() {
    if (DEBUG) printf("Time Variables:\n");
    for (int i = 0; i < n_variables; i++) {
        if (DEBUG) printf("  %s\n", variables[i]);
    }

    if (DEBUG) printf("Regular Variables:\n");
    for (int i = 0; i < n_parameters; i++) {
        if (DEBUG) printf("  %s\n", parameters[i]);
    }
}
/* Error function handler */
void yyerror(const char *s) {
    fprintf(stderr, "Sintax error: %s\n", s);
}

int yydebug = 1; // Debugging
int main() {
    // TODO receive equations by arguments
    if (DEBUG) printf("Please give me model equations in LaTeX:\n");
    yyparse();
    generate_code();  // Generate code from the parsed equations.
    return 0;
}