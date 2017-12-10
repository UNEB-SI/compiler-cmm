#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"semantic.h"
#include"lexical.h"

int contLabel = 0;
int contGo = 1;
int markID = 0;



int getLabel(){
        contLabel++;
        contGo = contLabel + 1;
        return contLabel;
}

int getGoTO(char value[]){
    printf("%s L%d\n",value,contGo);
    contGo++;
    return contGo-1;
}

void getLoadOrPush(Token t){
    //printf(" mark %d\n",markID);
   // if(markID <= 2){
        if(t.type == ID){ //Semantico
                printf("LOAD %s\n",t.lexem.value);

            }else if(t.type == INTCON){
                printf("PUSH %i\n",t.iValue);

            }else if(t.type == REALCON){
                printf("PUSH %lf\n",t.dValue);

            }else if(t.type == CARACCON){
                printf("PUSH %c\n",t.cValue);

            }else if(t.type == CADEIACON){
                printf("PUSH %s\n",t.word);
    /*        }
        markID++;
    }else{
        markID = 0;
    */}
}

void operator_check(Token t){
    int aux_z = 0,aux_x = 0, aux_y = 0;
printf("-------------------------------------------------------------\n");
    if(strcmp(t.signal,"==") == 0){ //Semantico
              aux_x = getLabel();
              printf("GOFALSE L%d\n",aux_x);

                printf("PUSH 0\n");

              aux_y = getLabel();
              printf("GOTO L%d\n",aux_y);

                printf("LABEL L%d\n",aux_x);
                printf("PUSH 1\n");
                printf("LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }else if(strcmp(t.signal,">=") == 0){
                printf("COPY\n");
                getGoTO("GOFALSE");
                getGoTO("GOTRUE");
                printf("PUSH 0\n");
                getGoTO("GOTO");
                getLabel();
                printf("POP\n");
                getLabel();
                printf("PUSH 1\n");
                getLabel();
                getGoTO("GOFALSE");

            }else if(strcmp(t.signal,"<=") == 0){
                    /*printf("COPY\n");
                    getGoTO("GOTRUE");
                    getGoTO("GOFALSE");
                    printf("PUSH 0\n");
                    getGoTO("GOTO");
                    getLabel();
                    printf("POP\n");
                    getLabel();
                    printf("PUSH 1\n");
                    getLabel();
                    getGoTO("GOFALSE");
                    */
            }else if(strcmp(t.signal,"<") == 0){
                printf("COPY\n");

               aux_x = getLabel();
               printf("GOFALSE L%d\n",aux_x);

               aux_y = getLabel();
               printf("GOTRUE L%d\n",aux_y);

                printf("PUSH 1\n");

               aux_z = getLabel();
               printf("GOTO L%d\n",aux_z);

                printf("LABEL L%d\n",aux_x);
                printf("POP\n");
                printf("LABEL L%d\n",aux_y);
                printf("PUSH 0\n");
                printf("LABEL L%d\n",aux_z);
                contLabel = aux_z;
            }else if(strcmp(t.signal,">") == 0){
                aux_x = getLabel();
                printf("GOTRUE L%d\n",aux_x);

                printf("PUSH 0\n");

                aux_y = getLabel();
                printf("GOTO L%d\n",aux_y);

                printf("LABEL L%d\n",aux_x);
                printf("PUSH 1\n");
                printf("LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }else if(strcmp(t.signal,"!=") == 0){

            }
printf("-------------------------------------------------------------\n");
}
