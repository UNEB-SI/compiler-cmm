#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "lexical.h"
#include "parser.h"
#include "sintatic_erros.h"

void prog() {
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = GLOBAL;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID){
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
                getToken();
                verifyRedeclaration(sb_token);
                insert_symbol();
                //verify if it is declaration
                while(token.type == SN && strcmp(token.signal, ",") == 0) {
                  getToken();
                  if(token.type == ID) {
                    strcpy(sb_token.name,token.lexem.value);
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
    printf("Sucesso na compilacao!\n");
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
}

int cmd(){
  //SE EXPRESSION
  if(token.type == PR && strcmp(token.pr, "se") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      expression expre = expr();

      if (strcmp(expre.type, "booleano") != 0) {
        printf ("Argumento imcopatível em expressão 'se' na linha %d\n", line_number);
        exit(-1);
      }

      if(token.type == SN && strcmp(token.signal,")") == 0){
        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        if(token.type == PR && strcmp(token.pr,"senao") == 0){
          getToken();
          if(!cmd()) {
            sintatic_erro(MISSING_CMD);
            exit(-1);
          }
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
      expression expre = expr();//add return

      if (strcmp(expre.type, "booleano") != 0) {
        printf ("Argumento imcopatível em expressão 'se' na linha %d\n", line_number);
        exit(-1);
      }

      if(token.type == SN && strcmp(token.signal,")") == 0){
        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
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
      if(token.type == SN && strcmp(token.signal,";") == 0){
        getToken();
        expression expre = expr();

        if (strcmp(expre.type, "booleano") != 0) {
          printf ("Argumento imcopatível em expressão 'se' na linha %d\n", line_number);
          exit(-1);
        }

        if(token.type == SN && strcmp(token.signal,";") == 0){
          getToken();
          atrib();
          if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            if(!cmd()){

              exit(-1);
            }
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

    expression expre = expr();

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
    getToken();
    getToken();
    char array_expression[50][50];
    expression express =  expr();
    strcpy(array_expression[0], express.type);
    int array_i = 1;
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      express = expr();
      strcpy(array_expression[array_i], express.type);
      array_i++;
    }
    //verify if all params matches
    validateParams(s, array_expression);
    if(token.type == SN && strcmp(token.signal, ")") == 0){
        getToken();
        if(token.type == SN && strcmp(token.signal, ";") == 0){
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
}

expression expr() {
  expression expre;
  expre = expr_simp();
  char operator[5];
  if(op_rel(operator)){
      expression expre2 = expr_simp();
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

expression expr_simp() {
  expression expre;
  char front_signal = '0';

  if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
    front_signal = token.signal[0];
    getToken();
  }

  expre = termo();

  while(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){
     //segura o sinal + ou -
      char signal = '0';

      if(strcmp(token.signal,"+") == 0) {
        signal = '+';
      } else if (strcmp(token.signal,"-") == 0) {
        signal = '-';
      }
      getToken();
      expression expre2 = termo();
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
              expre.bValue = 0;
            }
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
  if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
    strcpy(operator, token.signal);
    getToken();
    return 1;
  }
  return 0;
}

expression termo() {
  expression expr;
  expr = fator();
  while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){
      char signal = '0';
      //guardar sinal de * ou / ou &&
      if(strcmp(token.signal,"*") == 0) {
        signal = '*';
      } else if(strcmp(token.signal,"/") == 0) {
        signal = '/';
      }
      getToken();
      expression expr2 = fator();
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

expression fator() {
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
    expression express = expr();
    strcpy(array_expression[0], express.type);
    int array_i = 1;
    while(token.type == SN && strcmp(token.signal,",") == 0){
        getToken();
        express = expr();
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
    expre = expr();
    if(token.type == SN && strcmp(token.signal, ")") == 0) {
      getToken();
    } else {
      sintatic_erro(MISSING_CLOSE_PAREN);
      exit(-1);
    }
  }
  //NEGATION OF A FATOR
  else if(token.type == SN && strcmp(token.signal,"!") == 0){
    getToken();
    expression expre2 = fator();
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
  if(token.type == ID){
    symbol s = hasBeenDeclared(token.lexem.value);
    getToken();
    if(token.type == SN && strcmp(token.signal,"=") == 0){
      getToken();
      value = expr();
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
          s.cValue = value.iValue;
          s.init = 1;//mark that variable has been initialized
          updateVariableValue(s);
        } else if (strcmp(s.type, "inteiro") == 0  && strcmp(value.type, "caracter") == 0) {
          s.init = 1;//mark that variable has been initialized
          s.iValue = value.cValue;
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
