#ifndef STACKSEMANTIC_H
#define STACKSEMANTIC_H


typedef struct operators{
    char * type;
    int flag;
}operators;

FILE *stack_file;

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

void openStackFile();
int getLabel();
int getGoTO();
void getStoreID(char id[500]);
int loadLabelId(char id[]);
#endif // PARSER
