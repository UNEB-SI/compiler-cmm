#ifndef SEMANTIC_H
#define SEMANTIC_H

typedef struct operators{
    char * type;
    int flag;
}operators;

operators aux_operators;

int contLabel;
int contGoTo;
int markID; //Serve para ajudar a pegar o primeiro id da opera��o l�gica

int aux_store_go;
int aux_store_label;
int contAuxLabel;
int global_aux;

int getLabel();
int getGoTO();
#endif // PARSER