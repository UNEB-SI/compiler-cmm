#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

/*
  * Created by: Fernando Maia
  * Last Modification: 13/12/2017
*/

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

void push_value(expression value);
void open_stack_file();
void close_stack_file();
void operator_check_not_iqual();
int get_label();
int get_go_to();
void get_store_id(char id[500]);
int load_label_id(char id[]);
void get_load_or_push(Token t);
void operator_check(Token t);
#endif // STACK_MACHINE
