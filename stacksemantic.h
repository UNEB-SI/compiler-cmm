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
int markID; //Serve para ajudar a pegar o primeiro id da operação lógica

int aux_store_go;
int aux_store_label;
int contAuxLabel;
int global_aux;

int getLabel();
int getGoTO();
#endif // PARSER
