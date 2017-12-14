#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "symbol_table.h"

symbol last_function;
int param_mem = -3;

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
        int position = myFunctionHasPrototype(last_function);
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
    param_mem = -3;
    while(strcmp(symbol_table[i].name, "") != 0) {
      i++;
    }
    generate_mem_space(i, sb_token.cat);
    symbol_table[i] = sb_token;
  } else {
    while(strcmp(symbol_table[i].type, "") != 0) {
      i++;
    }
    if(sb_token.cat == PARAN) {
        generate_mem_space(i, sb_token.cat);
    } else {
      param_mem = -3;
    }
    symbol_table[i] = sb_token;
  }
}

void generate_mem_space(int position, int type) {

  char mem_position[12];
  char positions[12];

  if(type != PARAN) {
    sprintf(positions, "%d", position);
  } else {
      sprintf(positions, "%d", param_mem);
      param_mem--;
  }

  sprintf(mem_position, "%d", sb_token.scope);
  strcat(mem_position, ".");
  strcat(mem_position, positions);
  strcpy(sb_token.mem_space, mem_position);

}

void insert_param_on_prototype(int position) {
  int i = position + 1;
  while(strcmp(symbol_table[i].type, "") != 0) {
    if (symbol_table[i].cat != PARAN) {
      printf("Parâmetro não esperado na linha %d\n", line_number);
      exit(-1);
    } else if (!symbol_table[i].fullfill && symbol_table[i].cat == PARAN) {
        if(strcmp(symbol_table[i].type, sb_token.type) == 0) {
           generate_mem_space(i, sb_token.cat);
           symbol_table[i] = sb_token;
           symbol_table[i].fullfill = 1;
           return;
        } else {
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
  for(i = index; i < (int) (sizeof(symbol_table)/sizeof(*symbol_table)) - 1; i++){
    symbol_table[i] = symbol_table[i+1];
  }
}

void verifyRedeclaration(symbol sb) {
  int i = 0;

  while(strcmp(symbol_table[i].name, "") != 0) {
    //verify prototype Redeclaration
    if(sb.cat == FUNC && sb.zumbi) {
      if(symbol_table[i].scope == sb.scope && strcmp(symbol_table[i].name, sb.name) == 0
         && symbol_table[i].zumbi == sb.zumbi) {
            printf("Redeclaração da função '%s' na linha %d\n", sb.name, line_number);
            exit(-1);
         }
    } else {
      if(symbol_table[i].scope == sb.scope && strcmp(symbol_table[i].name, sb.name) == 0 && !symbol_table[i].zumbi) {
        printf("Redeclaração de '%s' na linha %d\n", sb.name, line_number);
        exit(-1);
      }
    }
    i++;
  }
}

int hasPrototype(symbol s) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(strcmp(symbol_table[i].name, s.name) == 0 && symbol_table[i].cat == FUNC && strcmp(symbol_table[i].type, s.type) == 0 && symbol_table[i].zumbi) {
      return i;
    } else if(strcmp(symbol_table[i].name, s.name) == 0 && strcmp(symbol_table[i].type, s.type) != 0) {
      printf("Esperado tipo '%s' para a função %s na linha %d\n", symbol_table[i].type, s.name, line_number);
      exit(-1);
    }
    i++;
  }
  return -1;
}

int myFunctionHasPrototype(symbol s) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
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

int hasPreviousBody(symbol s) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(strcmp(symbol_table[i].name, s.name) == 0 && symbol_table[i].cat == FUNC) {
      printf("Redeclaração da função %s na linha %d\n", s.name, line_number);
      exit(-1);
    }
    i++;
  }
  return 0;
}

symbol hasBeenDeclared(char* var) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if (strcmp(symbol_table[i].name, var) == 0
        && symbol_table[i].cat != FUNC) {
        return symbol_table[i];
    }
    i++;
  }
  printf("Variável '%s' não declarada na linha %d\n", var, line_number);
  exit(-1);
}

void updateVariableValue(symbol sb) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if (strcmp(symbol_table[i].name, sb.name) == 0
        && symbol_table[i].cat != FUNC && sb.scope == symbol_table[i].scope) {
        symbol_table[i] = sb;
        return;
    }
    i++;
  }
}

symbol functionHasBeenDeclared(char* var) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if (strcmp(symbol_table[i].name, var) == 0
        && symbol_table[i].cat == FUNC) {
        return symbol_table[i];
    }
    i++;
  }
  printf("Função '%s' não declarada na linha %d\n", var, line_number);
  exit(-1);
}

void functionHasReturn(char* var) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if (strcmp(symbol_table[i].name, var) == 0){
      if(strcmp(symbol_table[i].type, "semretorno") != 0) {
        return;
      } else {
        printf("Função '%s' não possui valor de retorno na linha %d\n", var,
               line_number);
        exit(-1);
      }
    }
    i++;
  }
}

void functionHasNoReturn(char* var) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if (strcmp(symbol_table[i].name, var) == 0){
      if(strcmp(symbol_table[i].type, "semretorno") == 0) {
        return;
      } else {
        printf("A chamada para a função '%s' não deve possuir valor de retorno na linha %d\n", var,
               line_number);
        exit(-1);
      }
    }
    i++;
  }
}

void verifyParams(symbol sb){
  int position = hasPrototype(sb);
  if(position != -1) {
    int i = position + 1;
    while(strcmp(symbol_table[i].name, "") != 0) {
      if(symbol_table[i].cat != PARAN){
        return;
      } else if(symbol_table[i].cat == PARAN) {
        if(!symbol_table[i].fullfill) {
          printf("Parâmetro do tipo '%s' esperado na função %s linha %d\n",
                 symbol_table[i].type, sb.name, line_number);
          exit(-1);
        }
      }
      i++;
    }
  } else {
    return;
  }
}

void validateParams(symbol sb, char params[][50]) {
  int i = 0;

  while(strcmp(symbol_table[i].name, "") != 0) {
   if(symbol_table[i].cat == FUNC && strcmp(symbol_table[i].name, sb.name) == 0 && strcmp(symbol_table[i].type, sb.type) == 0) {
      i = i + 1;
      break;
    }
    i++;
  }

  int param_position = 0;

  if (strcmp(params[param_position], "nothing") == 0) {
    if (symbol_table[i].cat == PARAN) {
      printf("Parâmetro esperado como argumento na chamada da função %s, linha %d\n", sb.name, line_number);
      exit(-1);
    }
  }

  while(strcmp(params[param_position], "") != 0 && strcmp(params[param_position], "nothing") != 0) {
    if ((strcmp(params[param_position], symbol_table[i].type) != 0) || (symbol_table[i].cat != PARAN)) {
      if(symbol_table[i].cat == PARAN) {
        printf("Esperado %s na função %s como %d argumento, linha %d\n", symbol_table[i].type, sb.name, param_position+1, line_number);
        exit(-1);
      } else {
        printf("Parâmetro não identificado como argumento da função %s, linha %d\n", sb.name, line_number);
        exit(-1);
      }
    }
    param_position++;
    i++;
  }
  if(symbol_table[i].cat == PARAN) {
    printf("Falta parâmetro do tipo '%s' na chamada da função %s na linha %d\n", symbol_table[i].type,sb.name, line_number);
    exit(-1);
  }
}

void hasMainFunction() {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(strcmp(symbol_table[i].name, "principal") == 0 && symbol_table[i].cat == FUNC) {
      return;
    }
    i++;
  }

  printf("Você esqueceu de declarar uma função 'principal'\n");
  exit(-1);
}

char* get_mem_space(char variable_name[]) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(strcmp(symbol_table[i].name, variable_name) == 0 && symbol_table[i].cat != FUNC) {
      return symbol_table[i].mem_space;
    }
    i++;
  }
  return NULL;
}
