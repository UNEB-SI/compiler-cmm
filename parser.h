#ifndef PARSER_H
#define PARSER_H

extern Token token, last_token;
extern int line_number;

int firstTime;

int isFirstTime();
void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
void expr();
void expr_simp();
void termo();
int fator();
void op_rel();
void atrib();
#endif // PARSER
