#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

enum token_cat{VAR,PARAN,FUNC};
typedef enum token_cat token_cat;

typedef struct{
    char name[500];
    int zumbi;
    token_cat cat;
    char type[10];
    int scope;
    int init;
    int matchParam;
    int fullfill;
    char mem_space[50];
    union {
      char cValue;
      float dValue;
      int iValue;
      int bValue;
    };
}symbol;


symbol sb_token;
extern symbol last_function;
symbol symbol_table[1000];
//-------------------
// Counter for index on symbol table
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
void generate_mem_space(int position, int type);
void verifyParams(symbol sb);
int hasPreviousBody(symbol s);
symbol hasBeenDeclared(char* var);
symbol functionHasBeenDeclared(char* var);
void functionHasReturn(char* var);
void functionHasNoReturn(char* var);
void updateVariableValue(symbol sb);
void validateParams(symbol sb, char params[][50]);
void hasMainFunction();
char* get_mem_space(char variable_name[]);
int myFunctionHasPrototype(symbol s);
//-------------------
//erro function;
#endif // TABLE
