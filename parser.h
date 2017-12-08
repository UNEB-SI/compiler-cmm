#ifndef PARSER_H
#define PARSER_H

#define GLOBAL 1
#define LOCAL 0

extern Token token;
extern int line_number;

enum token_cat{VAR,PARAN,FUNC};
typedef enum token_cat token_cat;

enum code_error{MISSING_SEMI_COLON = 1,MISSING_ID, MISSING_CLOSE_PAREN, MISSING_OPEN_PAREN,
MISSING_OPEN_KEY, MISSING_CLOSE_KEY, MISSING_CMD, MISSING_EQUAL_SNG, SYMBOL_NOT_RECOG, MISSING_TYPE,
MISSING_COMMA};


typedef struct{
    char name[500];
    int zumbi;
    token_cat cat;
    char type[10];
    int scope;
    int fullfill;
    float mem_space;
}symbol;

symbol sb_token;
symbol symbol_table[1000];
// Counter for index on symbol table
int cont_st;
void insert_symbol();
//-------------------
void exclude_local_symbol();
void refix_array(int index);
void insert_zombie();
void printf_symbol();
void verifyRedeclaration(symbol sb);
int hasPrototype(symbol s);
void insert_param_on_prototype(int position);
void default_insert_table();
//-------------------

//int firstTime;
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
