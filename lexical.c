#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "lexical.h"
#include "parser.h"

char actual_char;
char last_char;
int STATE = 0;
char buffer[50];
int buffer_position = 0;
int indetifier_position = 0;
int literal_position = 0;
int line_number = 1;

//reserved word
char *reserved_word[] = {"inteiro", "real", "caracter", "booleano", "se", "senao", "sem retorno", "enquanto", "para", "retorne", "semparam", "verdadeiro", "falso", "prototipo"};
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
        readFile(argv[1]);
    }else{
        errorMessage(ERROR_PASS_FILE);
    }
}


void readFile(char *file_name){
    file = fopen(file_name, "r");
    if(file != NULL){
        getToken();
        prog();
    }else{
        errorMessage(ERROR_NOT_FOUND_FILE);
    }
}

Token getToken() {
  int status;
  while ((actual_char = fgetc(file)) != EOF && (status = checkState(actual_char, file)) != HAS_TOKEN);

  if(actual_char == EOF) {
    token.type = eOF;
  }

  return token;
}

int checkState(char c, FILE *f){
    switch(STATE){
        case 0:
            if(isLetter(c)){
                STATE = 1;
                addLetter(c);
            }else if(isdigit(c)){
                STATE = 3;
                addLetter(c);
            }else if(c == SPACE || c == TAB || c == ENTER){
                STATE = 0;
                if(c == ENTER){
                    line_number++;
                }
            }else if(c == BAR){
                STATE = 8;
                addLetter(c);
                last_char = c;
            }else if(c == EOF){
                STATE = 0;
                return END_OF_FILE;
            }else if(c == '|'){
                STATE = 39;
                addLetter(c);
            }else if(c == '&'){
                STATE = 38;
                addLetter(c);
            }else if(c == APOSTROPHE){//apostrofo
                STATE = 24;
                addLetter(c);
            }else if(c == QUOTES){//aspas
                STATE = 25;
                //addLetter(c);
            }  else if(c == '<' || c == '>' || c == '!' || c == '='){
                STATE = 35;
                addLetter(c);
            }else if(c == '+' || c == '-' || c == '*'
                    || c == '[' || c == ']' || c == '{' || c == '}'
                    || c == '(' || c == ')' || c == ';' || c == ','){
                STATE = 0;
                addLetter(c);
                addStringFinal();
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else {
              printf("Symbol '%c' not recognized at line: %d\n",c,line_number);
              exit(0);
              return -1;
            }
            break;
        case 1:
            if(isLetter(c) || isdigit(c) || c == UNDERLINE){
                STATE = 1;
                addLetter(c);
            }else{
                addStringFinal();
                if(isReservedWord(buffer) != -1){
                    token.type = PR;
                    strcpy(token.pr, reserved_word[isReservedWord(buffer)]);
                }else{
                    strcpy(identifiers[indetifier_position], buffer);
                    token.type = ID;
                    token.lexem.table_position = indetifier_position;
                    strcpy(token.lexem.value, buffer);
                    indetifier_position++;
                }

                cleanBuffer(f, c);
                return HAS_TOKEN;
            }
            break;
        case 3:
            if(isdigit(c)){
                STATE = 3;
                addLetter(c);
            }else if(c == '.'){
                STATE = 5;
                addLetter(c);
            }else{
                addStringFinal();
                token.type = INTCON;
                token.iValue = getInteger();
                cleanBuffer(f, c);
                return HAS_TOKEN;
            }
            break;
        case 5:
            if(isdigit(c)){
                STATE = 6;
                ungetc(c, f);
            }else{
                errorMessage(ERROR_NUMBER_FLOAT_FORMAT);
            }
            break;
        case 6:
            if(isdigit(c)){
                STATE = 6;
                addLetter(c);
            }else{
                addStringFinal();
                //printToken(REALCON, c);
                token.type = REALCON;
                token.dValue = getFloat();
                cleanBuffer(f, c);
                return HAS_TOKEN;
            }
            break;
        case 8:
            if(c == '*'){
                STATE = 9;
                addLetter(c);
            }else{
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                STATE = 0;
                justCleanBuffer();
                return HAS_TOKEN;
            }
            break;
        case 9:
            if(isprint(c) && c != '*'){
                STATE = 10;
                addLetter(c);
            }else if(c == '*'){
                STATE = 11;
                addLetter(c);
            }else{
                //mensagem de erro
                printf("Error. Comentário não finalizado após '%s' na linha %d.\n", buffer,line_number);
                return -1;
            }
            break;
        case 10:
            if(c == '*'){
                STATE = 11;
                addLetter(c);
            }else if(isprint(c)){
                STATE = 10;
                addLetter(c);
            }
            break;
        case 11:
            if(c == '/'){//inverted bar
                addLetter(c);
                justCleanBuffer();
            }
            break;
        case 19:
            if(c == 'n' || c == '0'){
                addLetter(c);
                STATE = 26;
            }else{
                //mensagem de erro
                printf("Error. Caracter não esperado após %c na linha %d.\n",INVERTED_BAR, line_number);
                return -1;
            }
            break;
        case 24:
            if(c == INVERTED_BAR){
               addLetter(c);
               STATE = 19;
            }else if(c != APOSTROPHE && isprint(c)){
                addLetter(c);
                STATE = 26;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após ' na linha %d.\n", line_number);
                return -1;
            }
            break;
        case 25:
            if(isprint(c) || c == SPACE){
                addLetter(c);
                //FAZER AQUi
                STATE = 28;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após \"  na linha %d.\n",line_number);
                return -1;
            }
            break;
        case 26:
            if(c == APOSTROPHE){
                addLetter(c);
                token.type = CARACCON;
                strcpy(token.word, buffer);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu um \' na linha %d.\n",line_number);
                return -1;
            }
            break;
        case 28:
            if(c == QUOTES){
                strcpy(literals[literal_position], buffer);
                token.type = CADEIACON;
                token.lexem.table_position = literal_position;
                strcpy(token.lexem.value, buffer);
                literal_position++;
                justCleanBuffer();
                return HAS_TOKEN;
            } else if(isprint(c) || c == SPACE){
                addLetter(c);
                STATE = 28;
            }else if(c == EOF && last_char != QUOTES){
                //mensagem de erro
                printf("Error. Você esqueceu um \" na linha %d.\n",line_number);
            }
            break;
        case 35:
            if(c == '='){
                addLetter(c);
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                cleanBuffer(f, c);
                return HAS_TOKEN;
            }
            break;
        case 38:
            if(c == '&'){
                addLetter(c);
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Operador & não reconhecido na linha %d.\n", line_number);
            }
            break;
        case 39:
            if(c == '|'){
                addLetter(c);
                token.type = SN;
                strcpy(token.signal, signals[isSignal(buffer)]);
                justCleanBuffer();
                return HAS_TOKEN;
            }else{
                //mensagem de erro
                printf("Error. Operador | não reconhecido na linha %d.\n", line_number);
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
    for(int i = 0; i < (sizeof(reserved_word)/sizeof(*reserved_word)); i++){
        if(strcmp(word, reserved_word[i]) == 0){
            return i;
        }
    }

    return -1;
}

int isSignal(char *word){
    for(int i = 0; i < (sizeof(signals)/sizeof(*signals)); i++){
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
