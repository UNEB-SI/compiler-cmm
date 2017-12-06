#ifndef TABLE_H
#define TABLE_H


// Counter for index on symbol table
enum token_cat{VAR,PARAN,FUNC};
typedef enum token_cat token_cat;

typedef struct{
    char name[500];
    int zumbi;
    token_cat cat;
    char type[10];
    int scope;
}symbol;

symbol sb_token;
symbol symbol_table[1000];
//-------------------
void insert_symbol();
void exclude_local_symbol();
void refix_array(int index);
void insert_zombie();
void printf_symbol();
void verifyRedeclaration(symbol sb);
//-------------------

//erro function;
#endif // PARSER
