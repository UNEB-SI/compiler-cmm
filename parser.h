#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;

void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
void expr();
void expr_simp();
void termo();
void fator();
int op_rel();
int atrib();
#endif // PARSER
