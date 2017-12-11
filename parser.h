#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;



// Counter for index on symbol table

//-------------------
//-------------------

//int firstTime;
void sintatic_erro(int flag);
void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
void expr(int aux_and,int aux_or);
void expr_simp(int aux_and,int aux_or);
void termo(int aux_and,int aux_or);
void fator(int aux_and,int aux_or);
int op_rel();
int atrib();
#endif // PARSER
