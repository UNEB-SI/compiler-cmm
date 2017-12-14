#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"
#include "parser.h"
#include "table.h"
#include "sintatic_erros.h"
#include "stacksemantic.h"

/*Managers of the values of each letter in the file*/
char actual_char;
char last_char;

int STATE = 0;
char buffer[50];
int buffer_position = 0;
int indetifier_position = 0;
int literal_position = 0;
int line_number = 1;

int first_time = 0;

clock_t start, end;
double cpu_time_used;
//reserved word
char *reserved_word[] = {"inteiro", "real", "caracter", "booleano", "se", "senao", "semretorno", "enquanto", "para", "retorne", "semparam", "verdadeiro", "falso", "prototipo"};
//accept signals
char *signals[] = {">","<","<=", ">=", "!", "!=", ";",",", "&&","||","+","-","*","/", "[", "]", "(", ")", "{", "}", "=", "=="};
char *signalsName[] = {"MAIOR","MENOR","MENOR_QUE", "MAIOR_QUE", "NEG", "DIF", "PT_VIRG","VIRGULA", "E","OU","ADD","SUB","MULT","DIV", "COL_ABER", "COL_FEC", "PAREN_ABER", "PAREN_FEC", "CHAVE_ABER", "CHAVE_FEC", "ATRIBUICAO", "IGUALDADE"};
//accept constants
const char TAB = '\t';
const char ENTER = '\n';
const char SPACE = ' ';
const char UNDERLINE = '_';
const char BAR = '/';
const int INVERTED_BAR = 92;
const int APOSTROPHE = 39;
const int QUOTES = 34;
const int HAS_TOKEN = 2;
const char* ERROR_PASS_FILE = "Você deve indicar um arquivo para ser analisado. Ex: lexical namefile.cmm";
const char* ERROR_NOT_FOUND_FILE = "Arquivo não encontrado!";
const char* ERROR_NUMBER_FLOAT_FORMAT = "Esperado um número após ";

int END_OF_FILE = -1;

FILE *file;


int main(int argc, char **argv){
    if(argc > 1){
         openStackFile();
         printf("INIP\n");
         fprintf(stack_file,"INIP\n");
         readFile(argv[1]);
    }else{
        errorMessage(ERROR_PASS_FILE);
    }
}


void readFile(char *file_name){
    file = fopen(file_name, "r");
    if(file != NULL){
        start = clock();
        getToken();
        prog(0);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Executado em %f segundos.\n", cpu_time_used);
    }else{
        errorMessage(ERROR_NOT_FOUND_FILE);
    }

    fclose(file);
    //closeStackFile();
}

Token getToken() {
  int status;

  if(first_time > 1) {
    token = next_token;
  }

  while ((status = checkState(file) != HAS_TOKEN )&& (actual_char != EOF));

  if(isFirstTime()) {
    if(actual_char == EOF) {
        token.type = eOF;
    } else {
        token = next_token;
        first_time++;
        getToken();
    }
  } else {
      first_time++;
  }

  if(actual_char == EOF) {
    next_token.type = eOF;
  }

  return token;
}

int checkState(FILE *f){
    actual_char = fgetc(f);
    switch(STATE){
        case 0:
            if(isLetter(actual_char)){
                STATE = 1;
                addLetter(actual_char);
            }else if(isdigit(actual_char)){
                STATE = 3;
                addLetter(actual_char);
            }else if(actual_char == SPACE || actual_char == TAB || actual_char == ENTER){
                STATE = 0;
                if(actual_char == ENTER){
                    line_number++;
                }
            }else if(actual_char == BAR){
                STATE = 8;
                addLetter(actual_char);
            }else if(actual_char == EOF){
                STATE = 0;
                return END_OF_FILE;
            }else if(actual_char == '|'){
                STATE = 39;
                addLetter(actual_char);
            }else if(actual_char == '&'){
                STATE = 38;
                addLetter(actual_char);
            }else if(actual_char == APOSTROPHE){//apostrofo
                STATE = 24;
                //addLetter(c);
            }else if(actual_char == QUOTES){//aspas
                STATE = 25;
                //addLetter(c);
            }  else if(actual_char == '<' || actual_char == '>' || actual_char == '!' || actual_char == '='){
                STATE = 35;
                addLetter(actual_char);
            }else if(actual_char == '+' || actual_char == '-' || actual_char == '*'
                    || actual_char == '{' || actual_char == '}'
                    || actual_char == '(' || actual_char == ')' || actual_char == ';' || actual_char == ','){
                STATE = 0;
                addLetter(actual_char);
                addStringFinal();
                next_token.type = SN;
                strcpy(next_token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else {
              printf("Symbol '%c' not recognized at line: %d\n",actual_char,line_number);
              exit(-1);
            }
            break;
        case 1:
            if(isLetter(actual_char) || isdigit(actual_char) || actual_char == UNDERLINE){
                STATE = 1;
                addLetter(actual_char);
            }else{
                addStringFinal();
                if(isReservedWord(buffer) != -1){
                    next_token.type = PR;
                    strcpy(next_token.pr, reserved_word[isReservedWord(buffer)]);
                }else{
                    strcpy(identifiers[indetifier_position], buffer);
                    next_token.type = ID;
                    next_token.lexem.table_position = indetifier_position;
                    strcpy(next_token.lexem.value, buffer);
                    indetifier_position++;
                }

                cleanBuffer(f, actual_char);
                return HAS_TOKEN;
            }
            break;
        case 3:
            if(isdigit(actual_char)){
                STATE = 3;
                addLetter(actual_char);
            }else if(actual_char == '.'){
                STATE = 5;
                addLetter(actual_char);
            }else{
                addStringFinal();
                next_token.type = INTCON;
                next_token.iValue = getInteger();
                cleanBuffer(f, actual_char);
                return HAS_TOKEN;
            }
            break;
        case 5:
            if(isdigit(actual_char)){
                STATE = 6;
                ungetc(actual_char, f);
            }else{
                errorMessage(ERROR_NUMBER_FLOAT_FORMAT);
                exit(-1);
            }
            break;
        case 6:
            if(isdigit(actual_char)){
                STATE = 6;
                addLetter(actual_char);
            }else{
                addStringFinal();
                next_token.type = REALCON;
                next_token.dValue = getFloat();
                cleanBuffer(f, actual_char);
                return HAS_TOKEN;
            }
            break;
        case 8:
            if(actual_char == '*'){
                STATE = 9;
                addLetter(actual_char);
            }else{
              STATE = 0;
              addStringFinal();
              next_token.type = SN;
              strcpy(next_token.signal, signals[isSignal(buffer)]);
              cleanBuffer(f, actual_char);
              return HAS_TOKEN;
            }
            break;
        case 9:
            if(actual_char != '*'){
                STATE = 10;
                addLetter(actual_char);
                if(actual_char == '\n'){
                    line_number++;
                }
            }else if(actual_char == '*'){
                STATE = 11;
                addLetter(actual_char);
            }else{
                //mensagem de erro
                printf("Error. Comentário não finalizado após '%s' na linha %d.\n", buffer, line_number);
                exit(-1);
            }
            break;
        case 10:
            //treating \n
            if(actual_char == '*'){
                STATE = 11;
                addLetter(actual_char);
            }else if(isprint(actual_char) || actual_char == SPACE || actual_char == '\t' || actual_char == '\n'){
                STATE = 10;
                addLetter(actual_char);
                 if(actual_char == '\n'){
                    line_number++;
                }
            }else{
                //mensagem de erro
                printf("Error. Comentário não finalizado na linha %d.\n",line_number);
                exit(-1);
            }
            break;
        case 11:
            if(actual_char == '/'){//inverted bar
                addLetter(actual_char);
                justCleanBuffer();
            }else if(actual_char == '*'){
                addLetter(actual_char);
                STATE = 11;
            }else if(isprint(actual_char)){
                addLetter(actual_char);
                STATE = 10;
            }else{
                //mensagem de erro
                printf("Error. Comentário não finalizado após * na linha %d.\n",line_number);
                exit(-1);
            }
            break;
        case 19:
            if(actual_char == 'n' || actual_char == '0'){
                if(actual_char == 'n') actual_char = '\n';
                else actual_char = '\0';
                addLetter(actual_char);
                STATE = 26;
            }else{
                //mensagem de erro
                printf("Error. Caracter não esperado após %c na linha %d.\n",INVERTED_BAR, line_number);
                exit(-1);
            }
            break;
        case 24:
            if(actual_char == INVERTED_BAR){
               //addLetter(c);
               STATE = 19;
            }else if(actual_char != APOSTROPHE && isprint(actual_char)){
                addLetter(actual_char);
                STATE = 26;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após ' na linha %d.\n", line_number);
                exit(-1);
            }
            break;
        case 25:
            if(isprint(actual_char) || actual_char == SPACE){
                addLetter(actual_char);
                STATE = 28;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após \"  na linha %d.\n",line_number);
                exit(-1);
            }
            break;
        case 26:
            if(actual_char == APOSTROPHE){
                addLetter(actual_char);
                next_token.type = CARACCON;
                strcpy(next_token.word, buffer);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu um \' na linha %d.\n",line_number);
                exit(-1);
            }
            break;
        case 28:
            if(actual_char == QUOTES){
                strcpy(literals[literal_position], buffer);
                next_token.type = CADEIACON;
                next_token.lexem.table_position = literal_position;
                strcpy(next_token.lexem.value, buffer);
                literal_position++;
                justCleanBuffer();
                return HAS_TOKEN;
            } else if(isprint(actual_char) || actual_char == SPACE){
                addLetter(actual_char);
                STATE = 28;
            }else if(actual_char == EOF && last_char != QUOTES){
                //mensagem de erro
                printf("Error. Você esqueceu um \" na linha %d.\n",line_number);
            }
            break;
        case 35:
            if(actual_char == '='){
                addLetter(actual_char);
                next_token.type = SN;
                strcpy(next_token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                next_token.type = SN;
                strcpy(next_token.signal, signals[isSignal(buffer)]);
                cleanBuffer(f, actual_char);
                return HAS_TOKEN;
            }
            break;
        case 38:
            if(actual_char == '&'){
                addLetter(actual_char);
                next_token.type = SN;
                strcpy(next_token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Operador & não reconhecido na linha %d.\n", line_number);
                exit(-1);
            }
            break;
        case 39:
            if(actual_char == '|'){
                addLetter(actual_char);
                next_token.type = SN;
                strcpy(next_token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Operador | não reconhecido na linha %d.\n", line_number);
                exit(-1);
            }
            break;
    }

    return 0;
}

int isLetter(char letter){
    if(isalpha(letter)){
        return 1;
    }
    return 0;
}


int isReservedWord(char *word){
    int i;
    for(i = 0; i < (int) (sizeof(reserved_word)/sizeof(*reserved_word)); i++){
        if(strcmp(word, reserved_word[i]) == 0){
            return i;
        }
    }

    return -1;
}

int isSignal(char *word){
    int i;
    for(i = 0; i < (int) (sizeof(signals)/sizeof(*signals)); i++){
        if(strcmp(word, signals[i]) == 0){
            return i;
        }
    }

    return -1;
}

void errorMessage(const char *error){
    printf("Error: %s\n", error);
}

void addLetter(char c){
    buffer[buffer_position] = c;
    last_char = c;
    buffer_position++;
}

void addStringFinal(){
    buffer[buffer_position] = '\0';
}

void cleanBuffer(FILE *f, char c){
    buffer_position = 0;
    memset(&buffer[0], 0, sizeof(buffer));
    STATE = 0;
    if(c != EOF){
        ungetc(c, f);
    }
}

void justCleanBuffer(){
    buffer_position = 0;
    memset(&buffer[0], 0, sizeof(buffer));
    STATE = 0;
}

int getInteger(){
    int number = (int) strtol(buffer, (char **)NULL, 10);
    return number;
}

float getFloat(){
    float number = atof(buffer);
    return number;
}

int isFirstTime() {
  if (first_time == 0) {
    return 1;
  }

  return 0;
}
