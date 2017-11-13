#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;

enum token_cat{VAR,PARAN,FUNC};
 typedef enum token_cat token_cat;


typedef struct{
    char name[500];
    int zumbi;
    token_cat cat;
    char type[500];
    int scope;
}symbol;
symbol sb_token;
symbol symbol_table[1000];
int cont_st;

//-------------------
void insert_symbol();
void delete_symbol();
void printf_symbol();
//-------------------

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
