#ifndef AST_H
#define AST_H

#define MAX_EQUATIONS 100

/* Struct for equations */
typedef struct {
    char variable[50];
    char equation[256];
} Equation;

extern int eq_count;
extern Equation equations[MAX_EQUATIONS];



#define MAX_VARIABLES 100  // Tamaño máximo de los arrays

extern char *variables[MAX_VARIABLES];  // Array para variables de tiempo
extern int n_variables;               // Contador de variables de tiempo

extern char *parameters[MAX_VARIABLES];  // Array para variables regulares
extern int n_parameters;               // Contador de variables regulares



void generate_code();

#endif
