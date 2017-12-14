#ifndef STACKSEMANTIC_H
#define STACKSEMANTIC_H

FILE *stack_file;

typedef struct operators{
    char * type;
    int flag;
}operators;


typedef struct storeid{
    char idname[500];
    int labelnumber;
}storeid;


operators aux_operators;

int contLabel;
int contGoTo;
int markID; //Serve para ajudar a pegar o primeiro id da opera��o l�gica

int aux_store_go;
int aux_store_label;
int contAuxLabel;
int global_aux;

void PushValue(expression value);
void openStackFile();
void closeStackFile();
void operator_check_not_Iqual();
int getLabel();
int getGoTO();
void getStoreID(char id[500]);
int loadLabelId(char id[]);
#endif // PARSER
