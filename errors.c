#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"
#include "table.h"
#include "errors.h"

void error_message(int flag){
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
            printf("Simbolo n�o identificado na linha %d\n", line_number);
        break;
        case MISSING_TYPE:
            printf("Erro esperado tipo na linha %d\n", line_number);
        break;
        case MISSING_COMMA:
            printf("Erro esperado ',' na linha %d\n", line_number);
        break;
        case ERROR_PASS_FILE:
           printf("Você deve indicar um arquivo para ser analisado. Ex: lexical namefile.cmm");
        break;
        case ERROR_NOT_FOUND_FILE:
          printf("Arquivo não encontrado!");
        break;
        case ERROR_NUMBER_FLOAT_FORMAT:
          printf("Esperado um número após ");
        break;
        case WRONG_ATRIBUTION:
          printf("Atribuição inválida na linha %d\n", line_number);
        break;
    }
    exit(-1);
}

void error_comparation(char *first, char *second) {
  printf("Comparação não possível entre os tipos %s e %s na linha %d\n", first, second, line_number);
  exit(-1);
}

void error_return_not_expected(char *first, char *second) {
  printf("Retorno '%s' não esperado para a função %s\n",first,second);
  exit(-1);
}
