#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "lexical.h"
#include "parser.h"
#include "sintatic_erros.h"
#include "stacksemantic.h"

symbol last_function;
int flag = 1; //Flag para auxiliar na solução de um problema
//
void prog() {
   char auxIdStore[500];
   int amem = 0;
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = GLOBAL;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID){

      strcpy(auxIdStore,token.lexem.value);
      getToken();
      //just for symbol table
      if ((token.type == SN && strcmp(token.signal, ";") == 0) || (token.type == SN && strcmp(token.signal, ",") == 0)) {
        sb_token.cat = VAR;
        verifyRedeclaration(sb_token);
        insert_symbol();
      }
      // DEFAULT DECLARATION
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(token.type == ID) {
          amem++;
          strcpy(sb_token.name,token.lexem.value);
          verifyRedeclaration(sb_token);
          insert_symbol();
          getToken();
        } else {
            sintatic_erro(MISSING_ID);
          exit(-1);
        }
      }
      if(token.type == SN && strcmp(token.signal, ";") == 0) {//final of declaration
        amem++;
        if(amem > 0){
            fprintf(stack_file,"AMEM %d\n",amem);
            printf("AMEM %d\n",amem);
        }

        getToken();
        prog();

      }

      //DEFAULT FUNCTION
      else if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function

        getToken();
        sb_token.cat = FUNC;
        verifyRedeclaration(sb_token);
        insert_symbol();
        last_function = sb_token;
        //getStoreID(auxIdStore); //Store the name of function and its label to use on declarations
        printf("LOAD %s\n",get_mem_space(auxIdStore));
        fprintf(stack_file,"LOAD %s\n",get_mem_space(auxIdStore));
        types_param();
        //verify if all parameters was good
        verifyParams(last_function);
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {

            getToken();
            while(isType()){
              strcpy(sb_token.type, token.pr);
              getToken();
              //symbol table
              sb_token.scope = LOCAL;
              strcpy(sb_token.name, token.lexem.value);
              sb_token.zumbi = 0;// we gonna set the cat later
              sb_token.cat = VAR;

              if(token.type == ID){
                amem++;
                getToken();
                verifyRedeclaration(sb_token);
                insert_symbol();
                //verify if it is declaration
               amem = 0;
                while(token.type == SN && strcmp(token.signal, ",") == 0) {
                  getToken();
                  if(token.type == ID) {
                    strcpy(sb_token.name,token.lexem.value);
                    verifyRedeclaration(sb_token);
                    insert_symbol();
                    amem++;
                    getToken();
                  } else {
                    sintatic_erro(MISSING_ID);
                    exit(-1);
                  }
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                           amem++;
                           printf("AMEM %d\n",amem);
                           fprintf(stack_file,"AMEM %d\n",amem); //Goes to the function what have been called

                  getToken();
                } else {
                  sintatic_erro(MISSING_SEMI_COLON);
                  exit(-1);
                }
              }
            }
            //verify if has commands
            while(cmd());

            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              // keep all parameters and delete local variables
              insert_zombie();
              exclude_local_symbol();
              prog();
            } else {
              sintatic_erro(MISSING_CLOSE_KEY);
              exit(-1);
            }
          } else {
            sintatic_erro(MISSING_OPEN_KEY);
            exit(-1);
          }
        } else {
          sintatic_erro(MISSING_CLOSE_PAREN);
          exit(-1);
        }

      } else{
        sintatic_erro(SYMBOL_NOT_RECOG);
        exit(-1);
      }
    } else {
      sintatic_erro(MISSING_ID);
      exit(-1);
    }
  }
  // RECOGNIZE PROTOTYPE
  else if(token.type == PR && strcmp(token.pr, "prototipo") == 0) {
    //just to keep the type of last function
    char last_function_type[300];
    getToken();
    if(isType()) {
      strcpy(sb_token.type, token.pr);
      strcpy(last_function_type, token.pr);
      getToken();
      sb_token.scope = GLOBAL;
      strcpy(sb_token.name,token.lexem.value);
      sb_token.zumbi = 1;// we gonna set the cat later
      if(token.type == ID) {
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          sb_token.cat = FUNC;
          hasPreviousBody(sb_token);
          verifyRedeclaration(sb_token);
          insert_symbol();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              sb_token.scope = GLOBAL;
              strcpy(sb_token.name, token.lexem.value);
              sb_token.zumbi = 1;// we gonna set the cat later
              if(token.type == ID) {
                //keep function type
                strcpy(sb_token.type, last_function_type);
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  sb_token.cat = FUNC;
                  verifyRedeclaration(sb_token);
                  insert_symbol();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    sintatic_erro(MISSING_CLOSE_PAREN);
                    exit(-1);
                  }
                } else {
                  sintatic_erro(MISSING_OPEN_PAREN);
                  exit(-1);
                }
              } else {
                sintatic_erro(MISSING_ID);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITH TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              sintatic_erro(MISSING_SEMI_COLON);
              exit(-1);
            }
          } else {
            sintatic_erro(MISSING_CLOSE_PAREN);
            exit(-1);
          }
        } else {
          sintatic_erro(MISSING_OPEN_PAREN);
          exit(-1);
        }
      } else {
        sintatic_erro(MISSING_ID);
        exit(-1);
      }
    }
    //PROTOTYPE WITHOUT TYPE
    else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
      strcpy(sb_token.type, token.pr);
      strcpy(last_function_type, token.pr);
      getToken();
      sb_token.scope = GLOBAL;
      strcpy(sb_token.name,token.lexem.value);
      sb_token.zumbi = 1;// we gonna set the cat later
      if(token.type == ID){
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          sb_token.cat = FUNC;
          hasPreviousBody(sb_token);
          verifyRedeclaration(sb_token);
          insert_symbol();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              sb_token.scope = GLOBAL;
              strcpy(sb_token.name, token.lexem.value);
              sb_token.zumbi = 1;// we gonna set the cat later
              if(token.type == ID) {
                //keep function type
                strcpy(sb_token.type, last_function_type);
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  sb_token.cat = FUNC;
                  verifyRedeclaration(sb_token);
                  insert_symbol();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    sintatic_erro(MISSING_CLOSE_PAREN);
                    exit(-1);
                  }
                } else {
                  sintatic_erro(MISSING_OPEN_PAREN);
                  exit(-1);
                }
              } else {
                sintatic_erro(MISSING_ID);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITHOUT TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              sintatic_erro(MISSING_SEMI_COLON);
              exit(-1);
            }
          } else {
            sintatic_erro(MISSING_CLOSE_PAREN);
          }
        } else {
          sintatic_erro(MISSING_OPEN_PAREN);
          exit(-1);
        }
      } else {
        sintatic_erro(MISSING_ID);
        exit(-1);
      }
    }else {
      sintatic_erro(MISSING_TYPE);
      exit(-1);
    }
  }
  // RECOGNIZE FUNCTION WITHOUT RETURN
  else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = GLOBAL;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID) {
      sb_token.cat = FUNC;
      verifyRedeclaration(sb_token);
      insert_symbol();
      last_function = sb_token;
      getToken();
      if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
        getToken();
        types_param();
        //verify if all parameters was good
        verifyParams(last_function);
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {
            getToken();
            while(isType()){
              strcpy(sb_token.type, token.pr);
              getToken();
              if(token.type == ID && (next_token.type == SN && (strcmp(next_token.signal, ",") == 0 || strcmp(next_token.signal, ";") == 0))){
                sb_token.scope = LOCAL;
                strcpy(sb_token.name, token.lexem.value);
                sb_token.zumbi = 0;// we gonna set the cat later
                sb_token.cat = VAR;
                verifyRedeclaration(sb_token);
                insert_symbol();
                getToken();
                //verify if it is declaration
                while(token.type == SN && strcmp(token.signal, ",") == 0) {
                  getToken();
                  if(token.type == ID) {
                    strcpy(sb_token.name, token.lexem.value);
                    verifyRedeclaration(sb_token);
                    insert_symbol();
                    getToken();
                  } else {
                    sintatic_erro(MISSING_ID);
                    exit(-1);
                  }
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                  getToken();
                } else {
                  sintatic_erro(MISSING_SEMI_COLON);
                  exit(-1);
                }
              }
            }
            while(cmd());
            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              // keep all parameters and delete local variables
              insert_zombie();
              exclude_local_symbol();
              prog();
            } else {
                sintatic_erro(MISSING_CLOSE_KEY);
              exit(-1);
            }
          } else {
            sintatic_erro(MISSING_OPEN_PAREN);
            exit(-1);
          }
        } else {
          sintatic_erro(MISSING_CLOSE_PAREN);
          exit(-1);
        }

      } else{
        sintatic_erro(MISSING_OPEN_PAREN);
        exit(-1);
      }
    } else {
      sintatic_erro(MISSING_ID);
      exit(-1);
    }
  }
  else if(token.type == eOF){
    //verify if has program on the body
    hasMainFunction();
    printf("Compilado com sucesso!\n");
    fprintf(stack_file,"HALT\n");
    fclose(stack_file);
    return;
  }
  else {
    sintatic_erro(SYMBOL_NOT_RECOG);
    exit(-1);
  }
}

/**
* Verify the interval between reserved word to know if it is a type definition
**/
int isType() {
  if(token.type == PR && isReservedWord(token.pr) >= 0 && isReservedWord(token.pr) < 4){
    return 1;
  }
  return 0;
}

void types_param(){
    int amem = 0;
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = LOCAL;
    sb_token.cat = PARAN;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID){
      amem++;
      getToken();
      sb_token.cat = PARAN;
      verifyRedeclaration(sb_token);
      insert_symbol();
      //verify if it is declaration
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(isType()){
          strcpy(sb_token.type, token.pr);
          getToken();
          sb_token.scope = LOCAL;
          sb_token.cat = PARAN;
          strcpy(sb_token.name,token.lexem.value);
          sb_token.zumbi = 0;// we gonna set the cat later
          if(token.type == ID) {
            amem++;
            getToken();
            sb_token.cat = PARAN;
            verifyRedeclaration(sb_token);
            insert_symbol();
          }else {
            sintatic_erro(MISSING_ID);
            exit(-1);
          }
        } else {
            sintatic_erro(MISSING_COMMA);
          exit(-1);
        }
      }
    } else {
      sintatic_erro(MISSING_ID);
      exit(-1);
    }
  } else {
    sintatic_erro(SYMBOL_NOT_RECOG);
    exit(-1);
  }
  if(amem > 0){
    printf("AMEM %d\n",amem);
    fprintf(stack_file,"AMEM %d\n",amem);
  }
}

int cmd(){
    int aux_and = 0,aux_or = 0,labelid = 0;
    char functionValue[500];
  //SE EXPRESSION
  //variaveis para auxiliar o for
    int labelw = 0,labely = 0,labelz = 0,labelx = 0;
  //-----------------------------
  if(token.type == PR && strcmp(token.pr, "se") == 0){
   // getLabel();
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      aux_and = getLabel();
      aux_or = getLabel();
      expression expre = expr(aux_and, aux_or);
      printf("GOFALSE L%d\n",aux_and);
      fprintf(stack_file,"GOFALSE L%d\n",aux_and);
      if(token.type == SN && strcmp(token.signal,")") == 0){
        printf("LABEL L%d\n",aux_or);
        fprintf(stack_file,"LABEL L%d\n",aux_or);
        getToken();
        if(!cmd()){
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        if(token.type == PR && strcmp(token.pr,"senao") == 0){
          labelx = getLabel();
          printf("GOTO L%d\n",labelx);
          printf("LABEL L%d\n",aux_and);
          fprintf(stack_file,"GOTO L%d\n",labelx);
          fprintf(stack_file,"LABEL L%d\n",aux_and);
         // getLabel();
          getToken();
          if(!cmd()) {
            sintatic_erro(MISSING_CMD);
            exit(-1);
          }
          printf("LABEL L%d\n",labelx);
          fprintf(stack_file,"LABEL L%d\n",labelx);
        }else{
            printf("LABEL L%d\n",aux_and);
            fprintf(stack_file,"LABEL L%d\n",aux_and);
        }

        return 1;
      }else{
        sintatic_erro(MISSING_CLOSE_PAREN);
        exit(-1);
      }
    }else{
      sintatic_erro(MISSING_OPEN_PAREN);
      exit(-1);
    }
  }
  //ENQUANTO EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"enquanto") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
       aux_and = getLabel();
       aux_or = getLabel();
       printf("LABEL L%d\n",aux_and);
       fprintf(stack_file,"LABEL L%d\n",aux_and);
       expression expre = expr(aux_and, aux_or);//add return
       labely = getLabel();
       printf("GOFALSE L%d\n",labely);
       fprintf(stack_file,"GOFALSE L%d\n",labely);

      if (strcmp(expre.type, "booleano") != 0) {
        printf ("Argumento imcopatível em expressão 'se' na linha %d\n", line_number);
        exit(-1);
      }
      if(token.type == SN && strcmp(token.signal,")") == 0){
        printf("LABEL L%d\n",aux_or);
        fprintf(stack_file,"LABEL L%d\n",aux_or);
        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        printf("GOTO L%d\n",aux_and);
        printf("LABEL L%d\n",labely);
        fprintf(stack_file,"GOTO L%d\n",aux_and); //Aqui termina o while
        fprintf(stack_file,"LABEL L%d\n",labely);
        return 1;
      }else{
        sintatic_erro(MISSING_CLOSE_PAREN);
        exit(1);
      }
    }else{
      sintatic_erro(MISSING_OPEN_PAREN);
      exit(1);
    }
  }
  // PARA EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"para") == 0) {
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      atrib();
      labelw = getLabel();
      printf("LABEL L%d\n",labelw);
      fprintf(stack_file,"LABEL L%d\n",labelw);
      if(token.type == SN && strcmp(token.signal,";") == 0){
        getToken();
        labelx = getLabel();
        printf("GOFALSE L%d\n",labelx);
        fprintf(stack_file,"GOFALSE L%d\n",labelx);
        aux_or = getLabel();
        expression expre = expr(aux_and, aux_or);
        labely = getLabel();
        printf("GOTO L%d\n",labely);
        fprintf(stack_file,"GOTO L%d\n",labely);
        labelz = getLabel();
        printf("LABEL L%d\n",labelz);
        fprintf(stack_file,"LABEL L%d\n",labelz);

        if (strcmp(expre.type, "booleano") != 0) {
          printf ("Argumento imcopatível em expressão 'se' na linha %d\n", line_number);
          exit(-1);
        }

        if(token.type == SN && strcmp(token.signal,";") == 0){
          getToken();
          atrib();
          printf("GOTO L%d\n",labelw);
          printf("LABEL L%d\n",labely);
          fprintf(stack_file,"GOTO L%d\n",labelw);
          fprintf(stack_file,"LABEL L%d\n",labely);
          if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            printf("LABEL %d\n",aux_or);
            fprintf(stack_file,"LABEL %d\n",aux_or);
            if(!cmd()){
              exit(-1);
            }
            printf("GOTO L%d\n",labelz);
            printf("LABEL L%d",labelx);
            fprintf(stack_file,"GOTO L%d\n",labelz);
            fprintf(stack_file,"LABEL L%d",labelx);
            return 1;
          }else{
            sintatic_erro(MISSING_CLOSE_PAREN);
            exit(-1);
          }
        }else{
          sintatic_erro(MISSING_SEMI_COLON);
          exit(-1);
        }
      }else{
        sintatic_erro(MISSING_SEMI_COLON);
        exit(-1);
      }
    } else {
      sintatic_erro(MISSING_OPEN_PAREN);
      exit(-1);
    }
  }
  // RETORNE EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"retorne") == 0){
    printf("RET\n");
    fprintf(stack_file,"RET\n");
    getToken();
    //verify if function has return and if has, have to has a express returning
    if(strcmp(last_function.type, "semretorno") == 0) {
      if(strcmp(token.signal, ";") != 0) {
          printf("Retorno inesperado para a função '%s' na linha %d\n",
                 last_function.name, line_number);
          exit(-1);
      }
    } else {
      if(strcmp(token.signal, ";") == 0) {
          printf("Retorno '%s' esperado para a função '%s' na linha %d\n",
                 last_function.type, last_function.name, line_number);
          exit(-1);
      }
    }

    expression expre = expr(aux_and, aux_or);

    if(strcmp(expre.type, "nothing") != 0) {
      //verify if return type is equal to function type
      if ((strcmp(last_function.type, expre.type) != 0) && !(strcmp(last_function.type, "caracter") == 0 && strcmp(expre.type, "inteiro") == 0) && !(strcmp(last_function.type, "inteiro") == 0 && strcmp(expre.type, "caracter") == 0)){
        printf("Retorno '%s' não esperado para a função %s\n", expre.type, last_function.name);
        exit(-1);
      }
    }

    if(token.type == SN && strcmp(token.signal,";") == 0){
      getToken();
      return 1;
    }else{
      sintatic_erro(MISSING_SEMI_COLON);
      exit(-1);
    }
  }
  // KEY EXPRESSION
  else if(token.type == SN && strcmp(token.signal,"{") == 0){

    getToken();
    while(cmd());
    if(token.type == SN && strcmp(token.signal,"}") == 0) {
      getToken();
      return 1;
    } else {
      sintatic_erro(MISSING_CLOSE_KEY);
      exit(-1);
    }
  }
  // ; EXPRESSION
  else if(token.type == SN && strcmp(token.signal,";") == 0){
    getToken();
  }
  // FUNCTION CALL
  else if(token.type == ID && strcmp(next_token.signal,"(") == 0){
    symbol s = functionHasBeenDeclared(token.lexem.value);
    functionHasNoReturn(token.lexem.value);
    strcpy(functionValue,token.lexem.value);
    getToken();
    getToken();
    char array_expression[50][50];
    expression express =  expr(aux_and, aux_or);
    strcpy(array_expression[0], express.type);
    int array_i = 1;
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      express = expr(aux_and, aux_or);
      strcpy(array_expression[array_i], express.type);
      array_i++;
    }
    //verify if all params matches
    validateParams(s, array_expression);
    if(token.type == SN && strcmp(token.signal, ")") == 0){
        getToken();
        if(token.type == SN && strcmp(token.signal, ";") == 0){
          printf("CALL %s\n",get_mem_space(functionValue));
          fprintf(stack_file,"CAll %s\n",get_mem_space(functionValue));
          getToken();
          return 1;
        } else {
          sintatic_erro(MISSING_SEMI_COLON);
          exit(-1);
        }
    } else {
      // printf("Sou %d %s\n", token.type, token.signal);
      sintatic_erro(MISSING_OPEN_PAREN);
      exit(-1);
    }
  }
  // ATRIB EXPRESSION
  else if(atrib()){
    if(token.type == SN && strcmp(token.signal, ";") == 0) {
      getToken();

      return 1;
    } else {
      sintatic_erro(MISSING_SEMI_COLON);
      exit(-1);
    }
  }
  return 0;
}

void opc_p_types() {
    int amem = 0;
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    strcpy(sb_token.type, token.pr);
    sb_token.cat = PARAN;
    sb_token.scope = LOCAL;
    sb_token.zumbi = 0;
    sb_token.fullfill = 0;
    getToken();
    if(token.type == ID) {
      //keep identify
      strcpy(sb_token.name, token.lexem.value);
      sb_token.zumbi = 1;
      getToken();
      amem++;
    } else {
        strcpy(sb_token.name, " ");
    }
    //insert into symbol table
    insert_symbol();
    //verify if it is declaration
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      if(isType()){
        strcpy(sb_token.type, token.pr);
        sb_token.cat = PARAN;
        sb_token.scope = LOCAL;
        sb_token.zumbi = 0;
        sb_token.fullfill = 0;
        getToken();
        if(token.type == ID) {
          //keep identify
          amem++;
          strcpy(sb_token.name, token.lexem.value);
          sb_token.zumbi = 1;
          getToken();
        } else {
            strcpy(sb_token.name, " ");
        }
        //insert into symbol table
        insert_symbol();
      } else {
        sintatic_erro(MISSING_TYPE);
        exit(-1);
      }
    }
  } else {
    sintatic_erro(SYMBOL_NOT_RECOG);
    exit(-1);
  }
  if(amem > 0){
    printf("AMEM %d\n",amem);
    fprintf(stack_file,"AMEM %d\n",amem);
  }
}

expression expr(int aux_and, int aux_or) {
  expression expre;
  expre = expr_simp(aux_and, aux_or);
  char operator[5];
  if(op_rel(operator)){
      expression expre2 = expr_simp(aux_and, aux_or);
      if((strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) || (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0)
          || (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0) || (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0)
          || (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0)) {
            if(strcmp(operator, "==") == 0) {
               if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                  if (expre.iValue == expre2.iValue) {
                    strcpy(expre.type, "booleano");
                    expre.bValue = 1;
                  } else {
                    strcpy(expre.type, "booleano");
                    expre.bValue = 0;
                  }
               }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                 if (expre.dValue == expre2.dValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
               } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                 if (expre.cValue == expre2.cValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
               } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                 if (expre.cValue == expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
               } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                 if (expre.iValue == expre2.cValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
               }
            } else if (strcmp(operator, "!=") == 0) {
              if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                 if (expre.iValue != expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
              }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                if (expre.dValue != expre2.dValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.cValue != expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                if (expre.cValue != expre2.iValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.iValue != expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              }
            } else if (strcmp(operator, "<=") == 0) {
              if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                 if (expre.iValue <= expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
              }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                if (expre.dValue <= expre2.dValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.cValue <= expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                if (expre.cValue <= expre2.iValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.iValue <= expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              }
            } else if (strcmp(operator, "<") == 0) {
              if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                 if (expre.iValue < expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
              }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                if (expre.dValue < expre2.dValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.cValue < expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                if (expre.cValue < expre2.iValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.iValue < expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              }
            } else if (strcmp(operator, ">") == 0) {
              if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                 if (expre.iValue > expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
              }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                if (expre.dValue > expre2.dValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.cValue > expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                if (expre.cValue > expre2.iValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.iValue > expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              }
            } else if (strcmp(operator, ">=") == 0) {
              if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
                 if (expre.iValue >= expre2.iValue) {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 1;
                 } else {
                   strcpy(expre.type, "booleano");
                   expre.bValue = 0;
                 }
              }else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
                if (expre.dValue >= expre2.dValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.cValue >= expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0){
                if (expre.cValue >= expre2.iValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0){
                if (expre.iValue >= expre2.cValue) {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 1;
                } else {
                  strcpy(expre.type, "booleano");
                  expre.bValue = 0;
                }
              }
            }
      } else {
          printf("Tipo %s não pode ser comparado com tipo %s na linha %d\n", expre.type, expre2.type, line_number);
          exit(-1);
      }
  }

  return expre;
}

expression expr_simp(int aux_and, int aux_or) {
  expression expre;
  char front_signal = '0';
  Token t = token;
  int cont_operator = 0;
  if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
    front_signal = token.signal[0];
    getToken();
    cont_operator++;
  }

  expre = termo(aux_and, aux_or);
//-------------------------------------------------
  if(cont_operator != 0){
      if(strcmp(t.signal,"+") == 0 ){
            printf("ADD\n");
            fprintf(stack_file,"ADD\n");
          }else if(strcmp(t.signal,"-") == 0){
            printf("SUB\n");
            fprintf(stack_file,"SUB\n");
          }
  }
//---------------------------------------------------

  while(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){
     //segura o sinal + ou -
      char signal = '0';
      if(strcmp(token.signal,"||") == 0){
        printf("COPY\n");
        printf("GOTRUE L%d\n",aux_or);
        printf("POP\n");
        fprintf(stack_file,"COPY\n");
        fprintf(stack_file,"GOTRUE L%d\n",aux_or);
        fprintf(stack_file,"POP\n");
      }
      t = token;

      if(strcmp(token.signal,"+") == 0) {
        signal = '+';
      } else if (strcmp(token.signal,"-") == 0) {
        signal = '-';
      }
      getToken();
      expression expre2 = termo(aux_and, aux_or);
      if(strcmp(t.signal,"+") == 0 ){
        printf("ADD\n");
        fprintf(stack_file,"ADD\n");
      }else if(strcmp(t.signal,"-") == 0){
        printf("SUB\n");
        fprintf(stack_file,"SUB\n");
      }
      //se há conta faça-a
      if(signal != '0') {
          //it is compatible
          if(strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
              if(front_signal == '-') {
                expre.iValue = expre.iValue * -1;
              }
              if(signal == '+') expre.iValue = expre.iValue + expre2.iValue;
              else if (signal == '-') expre.iValue = expre.iValue - expre2.iValue;

          } else if (strcmp(expre.type, "real") == 0 && strcmp(expre2.type, "real") == 0) {
              if(front_signal == '-') {
                expre.iValue = expre.iValue * -1;
              }
              if(signal == '+') expre.dValue = expre.dValue + expre2.dValue;
              else if(signal == '-') expre.dValue = expre.dValue - expre2.dValue;

          } else if ((strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "caracter") == 0) || (strcmp(expre.type, "caracter") == 0 && strcmp(expre2.type, "inteiro") == 0) || (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "caracter") == 0)) {
             strcpy(expre.type, "inteiro");
             int first_value = expre.cValue;
             if(front_signal == '-') {
               first_value = first_value * -1;
             }
             if(signal == '+') expre.iValue = first_value + expre2.cValue;
             else if(signal == '-') expre.iValue = first_value - expre2.cValue;

          } else {
            printf("Operação inválida entre os tipos %s e %s na linha %d\n", expre.type, expre2.type, line_number);
            exit(-1);
          }
      }

      if (strcmp(token.signal,"||") == 0) {
        if ((strcmp(expre.type, "inteiro") == 0 ||strcmp(expre.type, "booleano") == 0) && (strcmp(expre2.type, "inteiro") == 0 ||strcmp(expre2.type, "booleano") == 0) ) {
          if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "inteiro") == 0) {
            if (expre.iValue || expre2.iValue) {
              strcpy(expre.type, "booleano");
              expre.bValue = 1;
            } else {
              strcpy(expre.type, "booleano");
              expre.bValue = 0;
            }
          } else if (strcmp(expre.type, "inteiro") == 0 && strcmp(expre2.type, "booleano") == 0) {
            if (expre.iValue || expre2.bValue) {
              strcpy(expre.type, "booleano");
              expre.bValue = 1;
            } else {
              strcpy(expre.type, "booleano");
            }
              expre.bValue = 0;
          } else if (strcmp(expre.type, "booleano") == 0 && strcmp(expre2.type, "inteiro") == 0) {
            if (expre.bValue || expre2.iValue) {
              strcpy(expre.type, "booleano");
              expre.bValue = 1;
            } else {
              strcpy(expre.type, "booleano");
              expre.bValue = 0;
            }
          }
        } else {
          printf("Comparação não possível entre os tipos %s e %s na linha %d\n", expre.type, expre2.type, line_number);
          exit(-1);
        }
      }
  }

  return expre;
}

int op_rel(char operator[]){
    Token aux_token;
  if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
    aux_token = token;
    strcpy(operator, token.signal);
    getToken();
    getLoadOrPush(token);
    printf("SUB\n");
    fprintf(stack_file,"SUB\n");
    operator_check(aux_token);
    flag = 0;
    return 1;
  }
  return 0;
}

expression termo(int aux_and, int aux_or) {
  expression expr;
  Token t;
  expr = fator(aux_and, aux_or);
  t = token;
  while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){
      if(strcmp(token.signal,"&&") == 0){
        printf("COPY\n");
        printf("GOFALSE L%d\n",aux_and);
        printf("POP\n");
        fprintf(stack_file,"COPY\n");
        fprintf(stack_file,"GOFALSE L%d\n",aux_and);
        fprintf(stack_file,"POP\n");
      }
      char signal = '0';
      //guardar sinal de * ou / ou &&
      if(strcmp(token.signal,"*") == 0) {
        signal = '*';
      } else if(strcmp(token.signal,"/") == 0) {
        signal = '/';
      }
      getToken();
      expression expr2 = fator(aux_and, aux_or);

      if(strcmp(t.signal,"*") == 0 ){
        printf("MUL\n");
        fprintf(stack_file,"MUL\n");
      }else if(strcmp(t.signal,"/") == 0){
        printf("DIV\n");
        fprintf(stack_file,"DIV\n");
      }
      //se há conta faça-a
      if(signal != '0') {
          //it is compatible
          if(strcmp(expr.type, "inteiro") == 0 && strcmp(expr2.type, "inteiro") == 0) {
              if(signal == '*') expr.iValue = expr.iValue * expr2.iValue;
              else if (signal == '/') expr.iValue = expr.iValue / expr2.iValue;

          } else if (strcmp(expr.type, "real") == 0 && strcmp(expr2.type, "real") == 0) {
              if(signal == '*') expr.dValue = expr.dValue * expr2.dValue;
              else if(signal == '/') expr.dValue = expr.dValue / expr2.dValue;

          } else if ((strcmp(expr.type, "caracter") == 0 && strcmp(expr2.type, "caracter") == 0) || (strcmp(expr.type, "caracter") == 0 && strcmp(expr2.type, "inteiro") == 0) || (strcmp(expr.type, "inteiro") == 0 && strcmp(expr2.type, "caracter") == 0)) {
             strcpy(expr.type, "inteiro");
             if(signal == '*') expr.iValue = expr.cValue * expr2.cValue;
             else if(signal == '/') expr.iValue = expr.cValue / expr2.cValue;

          } else {
            printf("Operação inválida entre os tipos %s e %s na linha %d\n", expr.type, expr2.type, line_number);
            exit(-1);
          }
      }

      if (strcmp(token.signal,"&&") == 0) {
        if ((strcmp(expr.type, "inteiro") == 0 ||strcmp(expr.type, "booleano") == 0) && (strcmp(expr2.type, "inteiro") == 0 ||strcmp(expr2.type, "booleano") == 0) ) {
          if (strcmp(expr.type, "inteiro") == 0 && strcmp(expr2.type, "inteiro") == 0) {
            if (expr.iValue && expr2.iValue) {
              strcpy(expr.type, "booleano");
              expr.bValue = 1;
            } else {
              strcpy(expr.type, "booleano");
              expr.bValue = 0;
            }
          } else if (strcmp(expr.type, "inteiro") == 0 && strcmp(expr2.type, "booleano") == 0) {
            if (expr.iValue && expr2.bValue) {
              strcpy(expr.type, "booleano");
              expr.bValue = 1;
            } else {
              strcpy(expr.type, "booleano");
              expr.bValue = 0;
            }
          } else if (strcmp(expr.type, "booleano") == 0 && strcmp(expr2.type, "inteiro") == 0) {
            if (expr.bValue && expr2.iValue) {
              strcpy(expr.type, "booleano");
              expr.bValue = 1;
            } else {
              strcpy(expr.type, "booleano");
              expr.bValue = 0;
            }
          }
        } else {
          printf("Comparação não possível entre os tipos %s e %s na linha %d\n", expr.type, expr2.type, line_number);
          exit(-1);
        }
      }
  }
  return expr;
}

expression fator(int aux_and, int aux_or) {
  expression expre;
  strcpy(expre.type, "nothing");
  // FUNCTION CALL
  if(token.type == ID && next_token.type == SN && strcmp(next_token.signal,"(") == 0) {
    symbol s = functionHasBeenDeclared(token.lexem.value);
    functionHasReturn(token.lexem.value);
    strcpy(expre.type, s.type);
    getToken();
    getToken();
    char array_expression[50][50];
    expression express = expr(aux_and, aux_or);
    strcpy(array_expression[0], express.type);
    int array_i = 1;
    while(token.type == SN && strcmp(token.signal,",") == 0){
        getToken();
        express = expr(aux_and, aux_or);
        strcpy(array_expression[array_i], express.type);
        array_i++;
    }
    //verify if all params matches
    validateParams(s, array_expression);
    //close function
    if(token.type == SN && strcmp(token.signal,")") == 0){
      getToken();
    }else{
      sintatic_erro(MISSING_CLOSE_PAREN);
      exit(-1);
    }
  }
  // CONSTANTS TYPE
  else if(token.type == INTCON || token.type == REALCON || token.type == CARACCON || token.type  == CADEIACON || token.type == ID){

    if(flag == 1){
        getLoadOrPush(token);
    }else{
        flag = 1;
    }
    symbol s;
    //keep the values to return
    switch (token.type) {
      case ID:
        s = hasBeenDeclared(token.lexem.value);
        strcpy(expre.type, s.type);
        if(s.init != 0) {
          if(strcmp(s.type, "inteiro") == 0) {
            expre.iValue = s.iValue;
          } else if (strcmp(s.type, "real") == 0) {
            expre.dValue = s.dValue;
          } else if (strcmp(s.type, "caracter") == 0) {
            expre.cValue = s.cValue;
          }
        } else {
          printf("Variável '%s' não inicializada na linha %d\n", s.name, line_number);
          exit(-1);
        }
      break;
      case INTCON:
        strcpy(expre.type, "inteiro");
        expre.iValue = token.iValue;
      break;
      case REALCON:
        strcpy(expre.type, "real");
        expre.dValue = token.dValue;
      break;
      case CARACCON:
        strcpy(expre.type, "caracter");
        expre.cValue = token.cValue;
        expre.iValue = token.cValue;
      break;
      case CADEIACON:
        strcpy(expre.type, "literal");
        strcpy(expre.word, s.name);
      break;
    }
    getToken();

  }
  // EXPRESSION BETWEEN PARENTHESES
  else if(token.type == SN && strcmp(token.signal, "(") == 0) {
    getToken();
    expre = expr(aux_and, aux_or);
    if(token.type == SN && strcmp(token.signal, ")") == 0) {
      getToken();
    } else {
      sintatic_erro(MISSING_CLOSE_PAREN);
      exit(-1);
    }
  }
  //NEGATION OF A FATOR
  else if(token.type == SN && strcmp(token.signal,"!") == 0){ //Tem que analisar isso aqui depois vei
    getToken();
    expression expre2 = fator(aux_and, aux_or);
    if(strcmp(expre2.type, "inteiro") == 0){
      if(!expre2.iValue) {
        strcpy(expre.type, "booleano");
        expre.bValue = 1;
      } else {
        strcpy(expre.type, "booleano");
        expre.bValue = 0;
      }
    } else if (strcmp(expre2.type, "booleano") == 0) {
      if(!expre2.bValue) {
        strcpy(expre.type, "booleano");
        expre.bValue = 1;
      } else {
        strcpy(expre.type, "booleano");
        expre.bValue = 0;
      }
    }
  }

  return expre;
}

int atrib(){
  expression value;
    Token aux_atrib;
  if(token.type == ID){
    symbol s = hasBeenDeclared(token.lexem.value);
    aux_atrib = token;
    getToken();
    if(token.type == SN && strcmp(token.signal,"=") == 0){
      getToken();
      value = expr(0, 0);
      getLoadOrPush(token);
      printf("STOR %s\n",get_mem_space(aux_atrib.lexem.value));
      fprintf(stack_file,"STOR %s\n",get_mem_space(aux_atrib.lexem.value));
      //verify if the types matches, if yes make attribution
      if(strcmp(s.type, value.type) == 0) {
         if(strcmp(s.type, "inteiro") == 0) {
           s.iValue = value.iValue;
         } else if(strcmp(s.type, "real") == 0) {
           s.dValue = value.dValue;
         } else if(strcmp(s.type, "caracter") == 0) {
           s.cValue = value.cValue;
         }
         s.init = 1;//mark that variable has been initialized
         updateVariableValue(s);
      } else {
        if(strcmp(s.type, "caracter") == 0  && strcmp(value.type, "inteiro") == 0) {
          s.cValue = (char) value.iValue;
          s.init = 1;//mark that variable has been initialized
          updateVariableValue(s);
        } else if (strcmp(s.type, "inteiro") == 0  && strcmp(value.type, "caracter") == 0) {
          s.init = 1;//mark that variable has been initialized
          s.iValue = (unsigned int) value.cValue;
          updateVariableValue(s);
        } else {
          printf("Atribuição não compatível entre os tipos %s e %s na linha %d\n", s.type, value.type, line_number);
          exit(-1);
        }
      }
      return 1;
    }else{
      sintatic_erro(MISSING_EQUAL_SNG);
      exit(-1);
    }
  }else {
    return 0;
  }
}
//-----------------------------------------------------
