#ifndef PARSER_H
#define PARSER_H

extern Token token, last_token;
extern int line_number;

//int firstTime;
void sintatic_erro(int flag);
void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
int expr();
void expr_simp();
void termo();
void fator();
int op_rel();
int atrib();
#endif // PARSER
