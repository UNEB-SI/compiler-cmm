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
        printf("LABEL L%d\n",contLabel);
        return contLabel;
}

int getGoTO(char value[]){
    printf("%s L%d\n",value,contGo);
    contGo++;
    return contGo-1;
}

void getLoadOrPush(Token t){
    if(markID < 2){
        if(token.type == ID){ //Semantico
                printf("LOAD %s\n",t.lexem.value);

            }else if(t.type == INTCON){
                printf("PUSH %d\n",t.iValue);

            }else if(t.type == REALCON){
                printf("PUSH %lf\n",t.dValue);

            }else if(t.type == CARACCON){
                printf("PUSH %c\n",t.cValue);

            }else if(t.type == CADEIACON){
                printf("PUSH %s\n",t.word);
            }
        markID++;
    }else{
        markID = 0;
    }

}

void operator_check(Token t){

    if(strcmp(t.signal,"==") == 0){ //Semantico
                getGoTO("GOFALSE");
                printf("PUSH 0\n");
                getGoTO("GOTO");
                getLabel();
                printf("PUSH 1\n");
                getLabel();
                getGoTO("GOFALSE");
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
                getGoTO("GOFALSE");
                getGoTO("GOTRUE");
                printf("PUSH 1\n");
                getGoTO("GOTO");
                getLabel();
                printf("POP\n");
                getLabel();
                printf("PUSH 0\n");
                getLabel();

            }else if(strcmp(t.signal,">") == 0){
                getGoTO("GOTRUE");
                printf("PUSH 0\n");
                getGoTO("GOTO");
                getLabel();
                printf("PUSH 1\n");
                getLabel();

            }else if(strcmp(t.signal,"!=") == 0){

            }
}
