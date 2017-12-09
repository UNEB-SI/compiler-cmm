#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "table.h"

symbol last_function;

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
        //sb_token.scope = LOCAL;
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
    generate_mem_space(i);
    symbol_table[i] = sb_token;
  } else {
    while(strcmp(symbol_table[i].type, "") != 0) {
      i++;
    }
    if(sb_token.cat == PARAN) generate_mem_space(i);
    symbol_table[i] = sb_token;
  }
}

void generate_mem_space(int position) {

  char mem_position[12];
  char positions[12];

  sprintf(positions, "%d", position);
  sprintf(mem_position, "%d", sb_token.scope);

  strcat(mem_position, ".");
  strcat(mem_position, positions);

  strcpy(sb_token.mem_space, mem_position);
}

void insert_param_on_prototype(int position) {
  int i = position + 1;
  while(strcmp(symbol_table[i].type, "") != 0) {
    if (symbol_table[i].cat == FUNC) {
      printf("Sou %s\n", sb_token.name);
      printf("Parâmetro não esperado na linha %d\n", line_number);
      exit(-1);
    } else if (!symbol_table[i].fullfill && symbol_table[i].cat == PARAN) {
        if(strcmp(symbol_table[i].type, sb_token.type) == 0) {
           float mem_position = i / 100;
           generate_mem_space(i);
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
