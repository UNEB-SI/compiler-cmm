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

if(token.type == PR && strcmp(token.pr, "se") == 0){
            getToken();
            if(token.type == SN && strcmp(token.signal,"(") == 0){
                  getToken();
                  expr();
                  if(token.type == SN && strcmp(token.signal,")") == 0){
                     getToken();
                     cmd();
                     getToken();
                        if(token.type == PR && strcmp(token.pr,"senao") == 0){
                                getToken();
                                cmd();
                        }
                        return 1;
                 }else{
                    printf("Erro no cmd, expressao incorreta para a condicional falta o ')' para fechar, na linha %d",line_number);
                    exit(1);
                 }
            }else{
              printf("Erro no cmd, nao foi encontrado o '(' apos o 'se' da condicional na linha %d",line_number);
              exit(1);
            }
      }else if(token.type == SN && strcmp(token.signal,"{") == 0){
           getToken();
           while(token.type != SN && strcmp(token.signal,"}") != 0){
            //   printf("token novo: %s",token.);
               cmd();
               getToken();

           }

           return 1;

      }else if(token.type == ID){
                    atrib();
                    if(token.type == SN && strcmp(token.signal,";") == 0){
                        getToken();
                    }else{
                        printf("Erro no cmd, esperado ';', psra finalizar atribuicao");
                        exit(1);
                    }
                    return 1;
                 }
   /* if(token.type == PR && strcmp(token.pr, "se") == 0){
            getToken();
            if(token.type == SN && strcmp(token.signal,"(") == 0){
                  getToken();
                  expr();
                  if(token.type == SN && strcmp(token.signal,")") == 0){
                     getToken();
                     cmd();
                     getToken();
                        if(token.type == PR && strcmp(token.pr,"senao") == 0){
                                getToken();
                                cmd();
                        }
                        return 1;
                 }else{
                    printf("Erro no cmd, expressao incorreta para a condicional falta o ')' para fechar, na linha %d",line_number);
                    exit(1);
                 }
            }else{
              printf("Erro no cmd, nao foi encontrado o '(' apos o 'se' da condicional na linha %d",line_number);
              exit(1);
            }
        }else if(token.type == PR && strcmp(token.pr,"enquanto") == 0){ // continua daqui
           getToken();
           if(token.type == SN && strcmp(token.signal,"(") == 0){
                   getToken();
                   expr();
               if(token.type == SN && strcmp(token.signal,")") == 0){
                   getToken();
                   cmd();
                   return 1;
               }else{
                   printf("erro no cmd, nao foi encontrado o ')' para fechar o comando 'enquanto' na linha %d",line_number);
                   exit(1);
               }
           }else{
               printf("erro no cmd, nao foi encontrado o '(' para iniciar o comando 'enquanto' na linha %d",line_number);
               exit(1);
           }
       }else if(token.type == PR && strcmp(token.pr,"para") == 0){
           getToken();
           if(token.type == SN && strcmp(token.signal,"(") == 0){
               getToken();

               if(token.type == ID){
                   atrib();
                   getToken();
               }

               if(token.type == SN && strcmp(token.signal,";") == 0){
                   getToken();
               }else{
                   printf("Erro no cmd, esperado ';' para fechar procedimento na linha %d",line_number);
                   exit(1);
               }

               if((token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)) || (token.type == INTCON ||token.type == CADEIACON || token.type == REALCON || token.type == CARACCON) ){
                   expr();
                   getToken();
               }

               if(token.type == SN && strcmp(token.signal,";") == 0){
                   getToken();
               }else{
                   printf("Erro no cmd, esperado ';' para fechar procedimento de empressao na linha %d",line_number);
                   exit(1);
               }

               if(token.type == ID){
                   atrib();
                   getToken();
               }

               if(token.type == SN && strcmp(token.signal,")") == 0){
                   cmd();
                   return 1;
               }else{
                   printf("Erro no cmd, esperado ')' para fechar na linha %d",line_number);
                   exit(1);
               }
           }
       }else if(token.type == PR && strcmp(token.pr,"retorne") == 0){
           getToken();
           if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
               expr();
           }

           if(token.type == SN && strcmp(token.signal,";") == 0){
               getToken();
           }else{
               printf("Erro no cmd, esperado ';' para retornar procedimento na linha %d",line_number);
               exit(1);
           }
       }else if(token.type == SN && strcmp(token.signal,"{") == 0){
           getToken();
           while(token.type != SN && strcmp(token.signal,"}") != 0){
            //   printf("token novo: %s",token.);
               cmd();
               getToken();

           }
           return 1;
       }else if(token.type == SN && strcmp(token.signal,";") == 0){
           getToken();
       }else if(token.type == ID && strcmp(next_token.signal,"(")){
           getToken();
           if(token.type == SN && strcmp(next_token.signal,"(")){
                getToken();
                if((token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)) || (token.type == INTCON ||token.type == CADEIACON || token.type == REALCON || token.type == CARACCON || token.type == ID)){
                    expr();
                    getToken();
                    while(token.type != SN && strcmp(token.signal,",") == 0){
                        expr();
                        getToken();
                    }
                }
                printf("ggyygy token %s",token.signal);
                if(token.type == SN && strcmp(token.signal,")") == 0){
                    getToken();
                    if(token.type == SN && strcmp(token.signal,";") == 0){
                        getToken();
                        return 0;
                    }else{
                        printf("Erro no cmd, esperado ';' para finalizar fim criacao de escopos de referencia na linha %d",line_number);
                        exit(1);
                    }
                }else{
                    printf("gygygy Erro no cmd, esperado ')' funcao mal estruturada na linha %d",line_number);
                    exit(1);
                }
           }else{
            printf("Erro");
            exit(1);
           }
       }else{
            atrib();
            if(token.type == SN && strcmp(token.signal,";") == 0){
                return 1;
            }else{
                printf("Erro no cmd, esperado ';', psra finalizar atribuicao");
                exit(1);
            }
       }
          return 0;*/
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

void expr() {
    expr_simp();
    if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
        op_rel();
        expr_simp();
    }
}

void expr_simp(){
    if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
        getToken();
}

    termo();

    getToken();

    if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){
        while(token.type != SN && (strcmp(token.signal,"+") != 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") != 0)){
            termo();
            getToken();
        }
    }
}

void op_rel(){ // Não acrescenta token novo
    if(token.type != SN && (strcmp(token.signal,"==") != 0 || strcmp(token.signal,"!=") != 0 || strcmp(token.signal,"<=") != 0 || strcmp(token.signal,"<") != 0 || strcmp(token.signal,">") != 0 || strcmp(token.signal,">=") != 0)){
        printf("Erro no op_rel, esperado operador logico na linha %d",line_number);
        exit(1);
    }else{
        getToken();
    }
}

void termo(){
    fator();
    if(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){
        while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){
            fator();
            getToken();
        }
    }
}

int fator(){

    if(token.type == ID && next_token.type == SN && strcmp(next_token.signal,"(") == 0){

            getToken();
            if(token.type == SN && strcmp(token.signal,"(") == 0){ //nao precisa de erro
                getToken();
                if((token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)) || (token.type == INTCON ||token.type == CADEIACON || token.type == REALCON || token.type == CARACCON || token.type == ID)){
                    expr();
                    getToken();
                    if(token.type == SN && strcmp(token.signal,",")){
                        while(token.type == SN && strcmp(token.signal,",")){
                            getToken();
                            expr();
                        }
                    }
/////////
                    if(token.type == SN && strcmp(token.signal,")")){
                        getToken();
                        return 1;
                    }else{
                        printf("Erro na fator, esperado ')' para fechar expressao na linha %d",line_number);
                        exit(1);
                    }
                }
            }

    }if(token.type == INTCON ||token.type == CADEIACON || token.type == REALCON || token.type == CARACCON || token.type == ID){

        return 1;
    }else if(token.type == SN && strcmp(token.signal,"(") == 0){
        getToken();
        expr();

        if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            return 1;
        }

    }else if(token.type == SN && strcmp(token.signal,"!") == 0){
        getToken();
        fator();
        return 1;
    }

    return 0;
}

void atrib(){
//    printf("token: %s",token.);
    if(token.type == ID){
            getToken();
        if(token.type == SN && strcmp(token.signal,"=") == 0){
            getToken();
            expr();
        }else{
            printf("Erro no atrib, esperado um sinal na linha %d",line_number);
            exit(1);
        }
    }else{
        printf("Erro no atrib, esperado identificador na linha %d",line_number);
        exit(1);
    }
}
