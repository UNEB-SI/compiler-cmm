#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"

void prog() {
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    getToken();
    if(token.type == ID){
      getToken();
      //DEFAULT DECLARATION
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(token.type == ID) {
          getToken();
        } else {
          printf("Identificador esperado na linha %d\n", line_number);
          exit(-1);
        }
        //if it is ; declaration finish
        if(token.type == SN && strcmp(token.signal, ";") == 0) {
          getToken();
          prog();
        }
      }

      if(token.type == SN && strcmp(token.signal, ";") == 0) {//final of declaration
        getToken();
        prog();
      }
      //DEFAULT FUNCTION
      else if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
        getToken();
        types_param();
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {
            getToken();
            while(isType()){
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
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                  getToken();
                } else {
                  printf("';' Esperado na linha %d\n", line_number);
                  exit(-1);
                }
              }
            }
            //verify if has commands
            while(cmd() != 0);

            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              prog();
            } else {
              printf("'}' Esperado na linha %d\n", line_number);
              exit(-1);
            }
          } else {
            printf("'{'Esperado na linha %d\n", line_number);
            exit(-1);
          }
        } else {
          printf("')'Esperado na linha %d\n", line_number);
          exit(-1);
        }

      } else{
        printf("Entrada inválida na linha %d\n", line_number);
        exit(-1);
      }
    } else {
      printf("Esperado identificador na linha %d\n", line_number);
      exit(-1);
    }
  }
  // RECOGNIZE PROTOTYPE
  else if(token.type == PR && strcmp(token.pr, "prototipo") == 0) {
    getToken();
    if(isType()) {
      getToken();
      if(token.type == ID) {
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              if(token.type == ID) {
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    printf("Esperado ')' na linha %d\n", line_number);
                    exit(-1);
                  }
                } else {
                  printf("Esperado '(' na linha %d\n", line_number);
                  exit(-1);
                }
              } else {
                printf("Esperado identificador na linha %d\n", line_number);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITH TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              printf("';' Esperando na linha %d\n", line_number);
              exit(-1);
            }
          } else {
            printf("Esperado ')' na linha %d\n", line_number);
            exit(-1);
          }
        } else {
          printf("Esperado '(' na linha %d\n", line_number);
          exit(-1);
        }
      } else {
        printf("Esperado identificador na linha %d\n", line_number);
        exit(-1);
      }
    }

    //PROTOTYPE WITHOUT TYPE
    else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
      getToken();
      if(token.type == ID) {
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              if(token.type == ID) {
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    printf("Esperado ')' na linha %d\n", line_number);
                    exit(-1);
                  }
                } else {
                  printf("Esperado '(' na linha %d\n", line_number);
                  exit(-1);
                }
              } else {
                printf("Esperado identificador na linha %d\n", line_number);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITHOUT TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              printf("';' Esperando na linha %d\n", line_number);
              exit(-1);
            }
          } else {
            printf("Esperado ')' na linha %d\n", line_number);
          }
        } else {
          printf("Esperado '(' na linha %d\n", line_number);
          exit(-1);
        }
      } else {
        printf("Esperado identificador na linha %d\n", line_number);
        exit(-1);
      }
    }
    else {
      printf("Tô pegando %d\n %s\n", token.type, token.lexem.value);
      printf("Esperado definição de tipo na linha %d\n", line_number);
      exit(-1);
    }
  }

  else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
    getToken();
    if(token.type == ID) {
      getToken();
      if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
        getToken();
        types_param();
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {
            getToken();
            while(isType()){
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
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                  getToken();
                } else {
                  printf("';' Esperado na linha %d\n", line_number);
                  exit(-1);
                }
              }
            }
            //verify if has commands
            while(cmd() != 0);

            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              prog();
            } else {
              printf("'}' Esperado na linha %d\n", line_number);
              exit(-1);
            }
          } else {
            printf("'{'Esperado na linha %d\n", line_number);
            exit(-1);
          }
        } else {
          printf("')'Esperado na linha %d\n", line_number);
          exit(-1);
        }

      } else{
        printf("Entrada inválida  na linha %d\n", line_number);
        exit(-1);
      }
    } else {
      printf("Esperado identificador na linha %d\n", line_number);
      exit(-1);
    }
  }
  else if(token.type == eOF){
    printf("fim do arquivo\n");
    exit(0);
  } else{
    printf("Entrada inválida na linha %d\n", line_number);
    exit(-1);
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

void types_param(){
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    getToken();
    if(token.type == ID){
      getToken();
      //verify if it is declaration
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(isType()){
          getToken();
          if(token.type == ID) {
            getToken();
          }else {
            printf("Identificador esperado na linha %d\n", line_number);
            exit(-1);
          }
        } else {
          printf("Erro esperado tipo na linha %d\n", line_number);
          exit(-1);
        }
      }
    } else {
      printf("Identificador esperado na linha %d\n", line_number);
      exit(-1);
    }
  } else {
    printf("Simbolo não identificado na linha %d\n", line_number);
    exit(-1);
  }
}

// TODO: Implement that shit
int cmd() {
  //getToken();
  return 0;
}

void opc_p_types() {
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    getToken();
    if(token.type == ID){
      getToken();
    }
    //verify if it is declaration
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      //printf("Sai daqui %d\n %s\n", token.type, token.lexem.value);
      if(isType()){
        getToken();
        if(token.type == ID) {
          getToken();
        }
      } else {
        printf("Erro esperado tipo na linha %d\n", line_number);
        exit(-1);
      }
    }
  } else {
    printf("Simbolo não identificado na linha %d\n", line_number);
    exit(-1);
  }
}
