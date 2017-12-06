#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "table.h"

//Holds symbol table position


//-----------------------------------------------------

void insert_symbol(){
    symbol_table[cont_st] = sb_token;
    cont_st++;
}

void printf_symbol(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    printf("ID: %s Zombie: %d\n", symbol_table[i].name,  symbol_table[i].zumbi);
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
