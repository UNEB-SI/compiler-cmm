#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;

// Counter for index on symbol table
//-------------------
//-------------------
void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
expression expr(int aux_and, int aux_or);
expression expr_simp(int aux_and, int aux_or);
expression termo(int aux_and, int aux_or);
expression fator(int aux_and, int aux_or);
int op_rel(char operator[]);
int atrib();
void sintatic_erro(int flag);
#endif // PARSER
