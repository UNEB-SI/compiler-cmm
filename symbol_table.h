#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

enum token_cat{VAR,PARAN,FUNC};
typedef enum token_cat token_cat;

typedef struct{
    char name[500];
    int zumbi;
    token_cat cat; //holds category
    char type[10]; //holds type
    int scope; //keeps identifier scope
    int init; // verify if a function has been loaded
    int matchParam; //boolean verify if
    int fullfill; //boolean helper to handle if a parameter was completed
    char mem_space[50];//keeps mem space
    union {
      char cValue; //caracter values
      float dValue; //float values
      int iValue; //integer values
      int bValue; //boolean values
    };
} symbol;


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
