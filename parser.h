#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;

typedef struct{
  char type[10];
  union {
    char cValue;
    float dValue;
    int iValue;
    int bValue;
    char signal[2];
    char word[1000];
  };
} expression;

void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();
expression expr();
expression expr_simp();
expression termo();
expression fator();
int op_rel();
int atrib();
#endif // PARSER
