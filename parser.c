#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"

void prog() {
  if(isType()){
    getToken();
    if(token.type == ID){
      getToken();
      //verify if it is declaration
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
          getToken();
          if(token.type == ID) {
            getToken();
          } else {
            printf("Identificador esperado na linha %d\n", line_number);
            exit(-1);
          }
          //if it is ; declaration finished
          if(token.type == SN && strcmp(token.signal, ";") == 0) {
              printf("Finish Declaration\n");
              getToken();
              prog();
          }
      }

      if(token.type == SN && strcmp(token.signal, ";") == 0) {//final of declaration
            printf("Finish Declaration\n");
            getToken();
            prog();
      } else if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
          printf("IM A FUNCTION\n");
          getToken();
          exit(0);//just for now change later
      } else{
        printf("JOY SOY: %d\n %s\n", token.type, token.pr);
        printf("Entrada inválida na linha %d\n", line_number);
      }
    } else {
      printf("Esperado identificador na linha %d\n", line_number);
    }
  } else {
    printf("Entrada inválida na linha %d\n", line_number);
  }
}

/**
* Verify the interval between reserved word to know if it is a type definition
**/
int isType() {
  if(token.type == PR && isReservedWord(token.pr) >= 0
                      && isReservedWord(token.pr) < 4){
    return 1;
  }
  return 0;
}
