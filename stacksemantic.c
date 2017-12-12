#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"stacksemantic.h"
#include"lexical.h"
#include"table.h"

int contLabel = 0;
int cont = 0;
int markID = 0;
FILE *stack_file;
//------------------------------------------------------------------------

storeid stack_storeid[1000];

void openStackFile(){

    stack_file = fopen("stack_file.txt", "w");

}
void getStoreID(char id[500]){

    strcpy(stack_storeid[cont].idname,id);
    stack_storeid[cont].labelnumber = getLabel();
    cont++;
}

int loadLabelId(char id[]){

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
int getLabel(){
        contLabel++;
        return contLabel;
}


void getLoadOrPush(Token t){
    //printf(" mark %d\n",markID);
   // if(markID <= 2){
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
    /*        }
        markID++;
    }else{
        markID = 0;
    */}
}




void operator_check(Token t){
    int aux_z = 0,aux_x = 0, aux_y = 0;

    if(strcmp(t.signal,"==") == 0){ //Semantico
                aux_x = getLabel();
                fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                fprintf(stack_file,"PUSH 0\n");
                aux_y = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_y);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"PUSH 1\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }else if(strcmp(t.signal,">=") == 0){
                fprintf(stack_file,"COPY\n");
                aux_x = getLabel();
                fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                aux_y = getLabel();
                fprintf(stack_file,"GOTRUE L%d\n",aux_y);
                fprintf(stack_file,"PUSH 0\n");
                aux_z = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_z);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"POP\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                fprintf(stack_file,"PUSH 1\n");
                fprintf(stack_file,"LABEL L%d\n",aux_z);
                contLabel = aux_z;

            }else if(strcmp(t.signal,"<=") == 0){
                aux_x = getLabel();
                fprintf(stack_file,"GOTRUE L%d\n",aux_x);
                fprintf(stack_file,"PUSH 1\n");
                aux_y = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_y);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"PUSH 0\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }else if(strcmp(t.signal,"<") == 0){
                fprintf(stack_file,"COPY\n");
                aux_x = getLabel();
                fprintf(stack_file,"GOFALSE L%d\n",aux_x);
                aux_y = getLabel();
                fprintf(stack_file,"GOTRUE L%d\n",aux_y);
                fprintf(stack_file,"PUSH 1\n");
                aux_z = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_z);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"POP\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                fprintf(stack_file,"PUSH 0\n");
                fprintf(stack_file,"LABEL L%d\n",aux_z);
                contLabel = aux_z;
            }else if(strcmp(t.signal,">") == 0){
                aux_x = getLabel();
                fprintf(stack_file,"GOTRUE L%d\n",aux_x);
                fprintf(stack_file,"PUSH 0\n");
                aux_y = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_y);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"PUSH 1\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }else if(strcmp(t.signal,"!=") == 0){ // precisa revisar
                aux_x = getLabel();
                fprintf(stack_file,"GOTRUE L%d\n",aux_x);
                fprintf(stack_file,"PUSH 1\n");
                aux_y = getLabel();
                fprintf(stack_file,"GOTO L%d\n",aux_y);
                fprintf(stack_file,"LABEL L%d\n",aux_x);
                fprintf(stack_file,"PUSH 0\n");
                fprintf(stack_file,"LABEL L%d\n",aux_y);
                contLabel = aux_y;
            }
}
