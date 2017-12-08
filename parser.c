#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "semantic.h"
//Holds symbol table position
int cont_st = 0;
symbol last_function;
int contAuxLabel = 0;

void prog() {
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    //just to keep the type of last function
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
      //DEFAULT DECLARATION
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
  //variaveis para auxiliar o for
    int labelw,labely,labelz,labelx;
  //-----------------------------
  if(token.type == PR && strcmp(token.pr, "se") == 0){
   // getLabel();
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      expr();
      labely = getGoTO("GOFALSE");
      if(token.type == SN && strcmp(token.signal,")") == 0){

        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        if(token.type == PR && strcmp(token.pr,"senao") == 0){
          labelx = getGoTO("GOTO");
          printf("LABEL L%d\n",labely);
          getLabel();
          getToken();
          if(!cmd()) {
            sintatic_erro(MISSING_CMD);
            exit(-1);
          }
          printf("LABEL L%d\n",labelx);
        }else{
            printf("LABEL L%d\n",labely);
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
      contAuxLabel = getLabel();
      getToken();
      expr();//add return
      labelw = getGoTO("GOFALSE");
      if(token.type == SN && strcmp(token.signal,")") == 0){
        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        printf("GOTO L%d\n",contAuxLabel); //Aqui termina o while
        printf("LABEL L%d\n",labelw);
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
  else if(token.type == PR && strcmp(token.pr,"para") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      atrib();
      labelw = getLabel();
      if(token.type == SN && strcmp(token.signal,";") == 0){
        getToken();
        expr();
        labelx = getGoTO("GOFALSE");
        labely = getGoTO("GOTO");
        labelz = getLabel();
        if(token.type == SN && strcmp(token.signal,";") == 0){
          getToken();
          atrib();
          printf("GOTO L%d\n",labelw);
          printf("LABEL L%d\n",labely);
          if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            if(!cmd()){
              exit(-1);
            }
            printf("GOTO L%d\n",labelz);
            printf("LABEL L%d",labelx);
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
    getToken();
    expr();
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
    getToken();
    getToken();
    expr();
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      expr();
    }
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

void expr() {
  expr_simp();
  if(op_rel()){
      expr_simp();
  }

}

void expr_simp(){
  if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
    getToken();
  }

  termo();

  while(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){
      getToken();
      termo();
  }
}

int op_rel(){ // Não acrescenta token novo
    Token aux_token;
  if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
    aux_token = token;
    getToken();
    getLoadOrPush(token);
    printf("SUB\n");
    operator_check(aux_token);
    return 1;
  }
  return 0;
}

void termo(){
int and_aux = 0;
  fator();
  while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){

      getToken();
      fator();

  }
}

void fator(){
  // FUNCTION CALL
  if(token.type == ID && next_token.type == SN && strcmp(next_token.signal,"(") == 0) {
    getToken();
    getToken();
    expr();
    while(token.type == SN && strcmp(token.signal,",") == 0){
        getToken();
        expr();
    }
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
    getLoadOrPush(token);
    getToken();
  }
  // EXPRESSION BETWEEN PARENTHESES
  else if(token.type == SN && strcmp(token.signal, "(") == 0) {
    getToken();
    expr();
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
    fator();
  }
}

int atrib(){
  if(token.type == ID){
    printf("LOAD %s\n",token.lexem.value);
    getToken();
    if(token.type == SN && strcmp(token.signal,"=") == 0){
      getToken();
      expr();
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
void insert_symbol() {
    //if it is function, verify if has a prototype
    if(sb_token.cat == FUNC) {
      int position = hasPrototype(sb_token);
      if(position != -1) {//has prototype just overwrite
          symbol_table[position] = sb_token;
          return;
      } else {
        default_insert_table();
      }
    } else if(sb_token.cat == PARAN) {
        int position = hasPrototype(last_function);
        if(position != -1) {
          insert_param_on_prototype(position);
          return;
        } else {
          default_insert_table();
        }
    } else if(sb_token.cat == VAR) {
      default_insert_table();
    }
}

void printf_symbol(){
  int i = 0;

  while(strcmp(symbol_table[i].name, "") != 0) {
    char type[500] = "";
    if(symbol_table[i].cat == FUNC) {
        strcpy(type, "Função");
    } else if(symbol_table[i].cat == VAR) {
      strcpy(type, "Variável");
    } else if (symbol_table[i].cat == PARAN) {
      strcpy(type, "Parametro");
    }
    printf("ID: %s Type: %s  Tipagem: %s\n", symbol_table[i].name, type, symbol_table[i].type);
    i++;
  }
}

void default_insert_table() {
  int i = 0;
  if(sb_token.cat == VAR) {
    while(strcmp(symbol_table[i].name, "") != 0) {
      i++;
    }
    symbol_table[i] = sb_token;
  } else {
    while(strcmp(symbol_table[i].type, "") != 0) {
      i++;
    }
    symbol_table[i] = sb_token;
  }
}

void insert_param_on_prototype(int position) {
  int i = position + 1;
  while(strcmp(symbol_table[i].type, "") != 0) {
    if (symbol_table[i].cat == FUNC) {
      printf("Parâmetro não esperado na linha %d\n", line_number);
      exit(-1);
    } else if (!symbol_table[i].fullfill && symbol_table[i].cat == PARAN) {
        if(strcmp(symbol_table[i].type, sb_token.type) == 0) {
           symbol_table[i] = sb_token;
           symbol_table[i].fullfill = 1;
           return;
        } else {
          printf("S: %s Symbol: %s\n", sb_token.type, symbol_table[i].type);
          printf("Esperado tipo %s, encontrado tipo %s em '%s' na linha %d\n", symbol_table[i].type, sb_token.type, last_function.name,line_number);
          exit(-1);
        }
    }
    i++;
  }
}

void insert_zombie(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].cat == PARAN) {
      symbol_table[i].zumbi = 1;
    }
    i++;
  }
}

void exclude_local_symbol(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].scope == LOCAL && !symbol_table[i].zumbi) {
      refix_array(i);
    } else {
      i++;
    }
  }
}

void refix_array(int index) {
  int i;
  for(i = index; i < (sizeof(symbol_table)/sizeof(*symbol_table)) - 1; i++){
    symbol_table[i] = symbol_table[i+1];
  }
}

void verifyRedeclaration(symbol sb) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].scope == sb.scope && strcmp(symbol_table[i].name, sb.name) == 0 && !symbol_table[i].zumbi) {
      printf("Redeclaração de '%s' na linha %d\n", sb.name, line_number);
      exit(-1);
    }
    i++;
  }
}

void sintatic_erro(int flag){
    switch (flag){
        case MISSING_SEMI_COLON:
            printf("';' Esperado na linha %d\n", line_number);
        break;
        case MISSING_ID:
            printf("Esperado identificador na linha %d\n", line_number);
        break;
        case MISSING_CLOSE_PAREN:
            printf("Esperado ')' na linha %d\n", line_number);
        break;
        case MISSING_OPEN_PAREN:
            printf("Esperado '(' na linha %d\n", line_number);
        break;
        case MISSING_OPEN_KEY:
            printf("'{' Esperado na linha %d\n", line_number);
        break;
        case MISSING_CLOSE_KEY:
            printf("'}' Esperado na linha %d\n", line_number);
        break;
        case MISSING_CMD:
            printf("Comando esperado na linha %d\n ", line_number);
        break;
        case MISSING_EQUAL_SNG:
            printf("Esperado sinal '=' na linha %d",line_number);
        break;
        case SYMBOL_NOT_RECOG:
            printf("Simbolo não identificado na linha %d\n", line_number);
        break;
        case MISSING_TYPE:
            printf("Erro esperado tipo na linha %d\n", line_number);
        break;
        case MISSING_COMMA:
            printf("Erro esperado ',' na linha %d\n", line_number);
        break;
    }
}

int hasPrototype(symbol s) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(strcmp(s.name, "fibbonaci") == 0) {
      printf("here");
    }
    if(strcmp(symbol_table[i].name, s.name) == 0 && symbol_table[i].cat == FUNC && strcmp(symbol_table[i].type, s.type) == 0) {
      return i;
    } else if(strcmp(symbol_table[i].name, s.name) == 0 && strcmp(symbol_table[i].type, s.type) != 0) {
      printf("Esperado tipo '%s' para a função %s na linha %d\n", symbol_table[i].type, s.name, line_number);
      exit(-1);
    }
    i++;
  }
  return -1;
}
