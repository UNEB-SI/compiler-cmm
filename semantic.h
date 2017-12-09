#ifndef SEMANTIC_H
#define SEMANTIC_H

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
