#ifndef PARSER_H
#define PARSER_H
//constants  to set and get scope
#define GLOBAL 1
#define LOCAL 0
//Came from lexical.h
extern Token token;
extern int line_number;
extern int cont_local_var;

void prog(); // manage all kind of global declaration and functions
int isType(); //verify if actual token is a type like "inteiro, real"
void types_param();  // holds params
int cmd(); //manager all commands and return if it is a command or not
void opc_p_types(); // holds optional params
expression expr(int aux_and, int aux_or);
expression expr_simp(int aux_and, int aux_or);
expression termo(int aux_and, int aux_or);
expression fator(int aux_and, int aux_or);
int op_rel(char operator[]);
int atrib();// check if a atribution statement is correct
#endif // PARSER
