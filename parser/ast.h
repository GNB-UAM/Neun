#ifndef AST_H
#define AST_H

#define MAX_EQUATIONS 100
#define DEBUG 0

/* Struct for equations */
typedef struct {
    char variable[50];
    char equation[256];
} Equation;


/* Struct for functions */
typedef struct {
    char name[50];
    char args[50][10];
} Function;

extern int eq_count;
extern Equation equations[MAX_EQUATIONS];

extern int incs_eq_count;
extern Equation incs_equations[MAX_EQUATIONS];


extern int fun_count;
extern Function functions[MAX_EQUATIONS];

extern int is_timevariable;

#define MAX_VARIABLES 100  // Tamaño máximo de los arrays

extern char *variables[MAX_VARIABLES];  // Array para variables de tiempo
extern int n_variables;               // Contador de variables de tiempo

extern char *parameters[MAX_VARIABLES];  // Array para variables regulares
extern int n_parameters;               // Contador de variables regulares



void generate_code();

#endif
