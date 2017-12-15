#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lexical.h"
#include"stack_machine.h"
#include "parser.h"
#include"symbol_table.h"

int contLabel = 0;
int cont = 0;
int markID = 0;
int param_stack = -3;
FILE *stack_file;
int cont_not_iqual = 0;
//------------------------------------------------------------------------

storeid stack_storeid[1000];

void open_stack_file(){
    stack_file = fopen("stack_file.txt", "w");
    fprintf(stack_file,"INIP\n");
}

void close_stack_file(){
   fclose(stack_file);
}

void push_value(expression value){
    if(strcmp(value.type,"inteiro") == 0){
        fprintf(stack_file,"PUSH %d\n",value.iValue);
    }else if(strcmp(value.type,"real") == 0){
        fprintf(stack_file,"PUSH %f\n",value.dValue);
    }else if(strcmp(value.type,"caracter") == 0){
        fprintf(stack_file,"PUSH %c\n",value.cValue);
    }else if(strcmp(value.type,"inteiro") == 0){
        fprintf(stack_file,"PUSH %d\n",value.bValue);
    }
}

void get_store_id(char id[500]){
    strcpy(stack_storeid[cont].idname,id);
    stack_storeid[cont].labelnumber = get_label();
    cont++;
}

int load_label_id(char id[]){

    int aux_storeid = 0;
    for(aux_storeid = 0; aux_storeid < cont;aux_storeid++){
        if(strcmp(stack_storeid[aux_storeid].idname,id) == 0){
            aux_storeid = stack_storeid[aux_storeid].labelnumber;
            break;
        }
    }
    return aux_storeid;
}
//--------------------------------------------------------------------------------
int get_label(){
        contLabel++;
        return contLabel;
}


void get_load_or_push(Token t){
        if(t.type == ID){ //Semantico
                fprintf(stack_file,"LOAD %s\n",get_mem_space(t.lexem.value));

            }else if(t.type == INTCON){
                fprintf(stack_file,"PUSH %i\n",t.iValue);

            }else if(t.type == REALCON){
                fprintf(stack_file,"PUSH %lf\n",t.dValue);

            }else if(t.type == CARACCON){
                fprintf(stack_file,"PUSH %c\n",t.cValue);

            }else if(t.type == CADEIACON){
                fprintf(stack_file,"PUSH %s\n",t.word);
            }
}

void operator_check(Token t){
    int aux_z = 0,aux_x = 0, aux_y = 0;

        if(strcmp(t.signal,"==") == 0){ //Semantico
                    aux_x = get_label();
                    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 0\n");
                    aux_y = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_y);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 1\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    contLabel = aux_y;
                }else if(strcmp(t.signal,">=") == 0){
                    fprintf(stack_file,"COPY\n");
                    aux_x = get_label();
                    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                    aux_y = get_label();
                    fprintf(stack_file,"GOTRUE L%d\n",aux_y);
                    fprintf(stack_file,"PUSH 0\n");
                    aux_z = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_z);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"POP\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    fprintf(stack_file,"PUSH 1\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_z);
                    contLabel = aux_z;

                }else if(strcmp(t.signal,"<=") == 0){
                    fprintf(stack_file,"COPY\n");
                    aux_x = get_label();
                    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                    aux_y = get_label();
                    fprintf(stack_file,"GOTRUE L%d\n",aux_y);
                    fprintf(stack_file,"POP\n");
                    fprintf(stack_file,"PUSH 0\n");
                    aux_z = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_z);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    fprintf(stack_file,"PUSH 1\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_z);
                    contLabel = aux_z;
                }else if(strcmp(t.signal,"<") == 0){
                    fprintf(stack_file,"COPY\n");
                    aux_x = get_label();
                    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                    aux_y = get_label();
                    fprintf(stack_file,"GOTRUE L%d\n",aux_y);
                    fprintf(stack_file,"PUSH 1\n");
                    aux_z = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_z);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"POP\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    fprintf(stack_file,"PUSH 0\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_z);
                    contLabel = aux_z;
                }else if(strcmp(t.signal,">") == 0){
                    aux_x = get_label();
                    fprintf(stack_file,"GOTRUE L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 0\n");
                    aux_y = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_y);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 1\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    contLabel = aux_y;
                }else if(strcmp(t.signal,"!=") == 0){ // precisa revisar
                    aux_x = get_label();
                    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 1\n");
                    aux_y = get_label();
                    fprintf(stack_file,"GOTO L%d\n",aux_y);
                    fprintf(stack_file,"LABEL L%d\n",aux_x);
                    fprintf(stack_file,"PUSH 0\n");
                    fprintf(stack_file,"LABEL L%d\n",aux_y);
                    contLabel = aux_y;
                }


                if(cont_not_iqual != 0){

                    operator_check_not_iqual();
                }
    }


void operator_check_not_iqual(){
    int aux_x = 0, aux_y = 0;
    cont_not_iqual = 0;
    aux_x = get_label();
    fprintf(stack_file,"GOFALSE L%d\n",aux_x);
    fprintf(stack_file,"PUSH 0\n");
    aux_y = get_label();
    fprintf(stack_file,"GOTO L%d\n",aux_y);
    fprintf(stack_file,"LABEL L%d\n",aux_x);
    fprintf(stack_file,"PUSH 1\n");
    fprintf(stack_file,"LABEL L%d\n",aux_y);
    contLabel = aux_y;
}
