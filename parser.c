#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "stacksemantic.h"
//Holds symbol table position

symbol last_function;


int flag = 1; //Flag para auxiliar na solução de um problema


void prog() {
   char auxIdStore[500];
   int amem = 0;
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    //just to keep the type of last function
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
      //DEFAULT DECLARATION
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
        if(amem > 0)
            fprintf(stack_file,"AMEM %d\n",amem);

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
            getStoreID(auxIdStore); //Store the name of function and its label to use on declarations
         fprintf(stack_file,"LABEL L%d\n",loadLabelId(auxIdStore));
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
  if(amem > 0)
    fprintf(stack_file,"AMEM %d\n",amem);

}

int cmd(){
    int aux_and = 0,aux_or = 0,labelid = 0;
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
      expr(aux_and,aux_or);
      fprintf(stack_file,"GOFALSE L%d\n",aux_and);
      if(token.type == SN && strcmp(token.signal,")") == 0){
        fprintf(stack_file,"LABEL L%d\n",aux_or);
        getToken();
        if(!cmd()){
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
        if(token.type == PR && strcmp(token.pr,"senao") == 0){
          labelx = getLabel();
          fprintf(stack_file,"GOTO L%d\n",labelx);
          fprintf(stack_file,"LABEL L%d\n",aux_and);
         // getLabel();
          getToken();
          if(!cmd()) {
            sintatic_erro(MISSING_CMD);
            exit(-1);
          }
          fprintf(stack_file,"LABEL L%d\n",labelx);
        }else{
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
       fprintf(stack_file,"LABEL L%d\n",aux_and);
      expr(aux_and,aux_or);//add return
       labely = getLabel();
       fprintf(stack_file,"GOFALSE L%d\n",labely);
      if(token.type == SN && strcmp(token.signal,")") == 0){
        fprintf(stack_file,"LABEL %d\n",aux_or);
        getToken();
        if(!cmd()) {
          sintatic_erro(MISSING_CMD);
          exit(-1);
        }
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
  else if(token.type == PR && strcmp(token.pr,"para") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      atrib();
      labelw = getLabel();
      fprintf(stack_file,"LABEL L%d\n",labelw);
      if(token.type == SN && strcmp(token.signal,";") == 0){
        getToken();
        labelx = getLabel();
        fprintf(stack_file,"GOFALSE L%d\n",labelx);
        aux_or = getLabel();
        expr(labelx,aux_or);
        labely = getLabel();
        fprintf(stack_file,"GOTO L%d\n",labely);
        labelz = getLabel();
        fprintf(stack_file,"LABEL L%d\n",labelz);
        if(token.type == SN && strcmp(token.signal,";") == 0){
          getToken();
          atrib();
          fprintf(stack_file,"GOTO L%d\n",labelw);
          fprintf(stack_file,"LABEL L%d\n",labely);
          if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            fprintf(stack_file,"LABEL %d\n",aux_or);
            if(!cmd()){
              exit(-1);
            }
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
    fprintf(stack_file,"RET\n");
    getToken();
    expr(aux_and,aux_or);
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
    labelid = loadLabelId(token.lexem.value);
    getToken();
    getToken();
    expr(aux_and,aux_or);

    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      expr(aux_and,aux_or);
    }
    if(token.type == SN && strcmp(token.signal, ")") == 0){
        getToken();
        if(token.type == SN && strcmp(token.signal, ";") == 0){
          fprintf(stack_file,"GOTO L%d\n",labelid);
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
    fprintf(stack_file,"AMEM %d\n",amem);
  }
}

void expr(int aux_and, int aux_or) {
  expr_simp(aux_and,aux_or);
  if(op_rel()){
      expr_simp(aux_and,aux_or);
      flag = 1;
  }

}

void expr_simp(int aux_and, int aux_or){
  Token t = token;
  if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
    getToken();
    if(strcmp(t.signal,"+") == 0 ){
        fprintf(stack_file,"ADD\n");
      }else if(strcmp(t.signal,"-") == 0){
        fprintf(stack_file,"SUB\n");
      }
  }

  termo(aux_and,aux_or);


while(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){

      if(strcmp(token.signal,"||") == 0){
        fprintf(stack_file,"COPY\n");
        fprintf(stack_file,"GOTRUE L%d\n",aux_or);
        fprintf(stack_file,"POP\n");
      }
      t = token;
      getToken();
      termo(aux_and,aux_or);
      if(strcmp(t.signal,"+") == 0 ){
        fprintf(stack_file,"ADD\n");
      }else if(strcmp(t.signal,"-") == 0){
        fprintf(stack_file,"SUB\n");
      }

  }
}

int op_rel(){ // Não acrescenta token novo  all right
    Token aux_token;
  if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
    aux_token = token;
    getToken();
    getLoadOrPush(token);
    fprintf(stack_file,"SUB\n");
    operator_check(aux_token);
    flag = 0;
    return 1;
  }
  return 0;
}

void termo(int aux_and,int aux_or){ // all right
  Token t;
  fator(aux_and,aux_or);
  t = token;
  while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){

      if(strcmp(token.signal,"&&") == 0){
        fprintf(stack_file,"COPY\n");
        fprintf(stack_file,"GOFALSE L%d\n",aux_and);
        fprintf(stack_file,"POP\n");
      }
      getToken();
      fator(aux_and,aux_or);

      if(strcmp(t.signal,"*") == 0 ){
        printf(stack_file,"MUL\n");
      }else if(strcmp(t.signal,"/") == 0){
        printf(stack_file,"DIV\n");
      }
  }
}

void fator(int aux_and, int aux_or){ //all right
  // FUNCTION CALL
  if(token.type == ID && next_token.type == SN && strcmp(next_token.signal,"(") == 0) {
    getToken();
    getToken();
    expr(aux_and,aux_or);
    while(token.type == SN && strcmp(token.signal,",") == 0){
        getToken();
        expr(aux_and,aux_or);
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

    if(flag == 1){
        getLoadOrPush(token);
    }else{
        flag = 1;
    }
    getToken();

  }
  // EXPRESSION BETWEEN PARENTHESES
  else if(token.type == SN && strcmp(token.signal, "(") == 0) {
    getToken();
    expr(aux_and,aux_or);
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
    fator(aux_and,aux_or);
  }
}

int atrib(){
    Token aux_atrib;
  if(token.type == ID){
    aux_atrib = token;
    getToken();
    if(token.type == SN && strcmp(token.signal,"=") == 0){
      getToken();
      expr(0,0);
      getLoadOrPush(token);
      printf(stack_file,"STOR %s\n",aux_atrib.lexem.value);
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
