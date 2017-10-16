#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "lexical.h"

char actual_char;
char last_char;
int STATE = 0;
char buffer[50];
int buffer_position = 0;
int indetifier_position = 0;
int literal_position = 0;
int line_number = 1;

int main(int argc, char **argv){
    if(argc > 1){
        readFile(argv[1]);
    }else{
        errorMessage(ERROR_PASS_FILE);
    }
}


void readFile(char *file_name){
    FILE *file = fopen(file_name, "r");
    if(file != NULL){
        while((actual_char = fgetc(file)) != EOF){
            if(checkState(actual_char, file) == -1){
                return;
            };
        }
        if(actual_char == EOF){
            checkState(actual_char, file);
        }
    }else{
        errorMessage(ERROR_NOT_FOUND_FILE);
    }
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
                printToken(eOF, c);
            }else if(c == '|'){
                STATE = 39;
                addLetter(c);
            }else if(c == '&'){
                STATE = 38;
                addLetter(c);
            }else if(c == 39){//apostrofo
                STATE = 24;
                addLetter(c);
            }else if(c == 34){//aspas
                STATE = 25;
                addLetter(c);
            }  else if(c == '<' || c == '>' || c == '!' || c == '='){
                STATE = 35;
                addLetter(c);
            }else if(c == '+' || c == '-' || c == '*'
                    || c == '[' || c == ']' || c == '{' || c == '}'
                    || c == '(' || c == ')' || c == ';' || c == ','){
                STATE = 0;
                addLetter(c);
                printToken(SN, c);
                justCleanBuffer();
            }
            break;
        case 1:
            if(isLetter(c) || isdigit(c) || c == UNDERLINE){
                STATE = 1;
                addLetter(c);
            }else{
                addStringFinal();
                if(isReservedWord(buffer) != -1){
                    printToken(PR, c);
                }else{
                    strcpy(&identifiers[indetifier_position], buffer);
                    printToken(ID, c);
                    indetifier_position++;
                }

                cleanBuffer(f, c);
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
                printToken(CT_I, c);
                cleanBuffer(f, c);
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
                printToken(CT_R, c);
                cleanBuffer(f, c);
            }
            break;
        case 8:
            if(c == '*'){
                STATE = 9;
                addLetter(c);
            }else{
                printToken(SN, last_char);
                STATE = 0;
                justCleanBuffer();
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
                printToken(COMMENT, c);
                justCleanBuffer();
            }
            break;
        case 24:
            if(isprint(c)){
                addLetter(c);
                STATE = 26;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após ' na linha %d.\n",line_number);
                return -1;
            }
            break;
        case 25:
            if(isprint(c) || c == SPACE){
                //addLetter(c);
                //FAZER AQUi
                STATE = 28;
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu algo após \"  na linha %d.\n",line_number);
                return -1;
            }
            break;
        case 26:
            if(c == 39){
                addLetter(c);
                printToken(CT_C, c);
                justCleanBuffer();
            }else{
                //mensagem de erro
                printf("Error. Você esqueceu um \' na linha %d.\n",line_number);
                return -1;
            }
            break;
        case 28:
            if(c == 34){
                //addLetter(c);
                strcpy(&literals[literal_position], buffer);
                printToken(CT_L, c);
                literal_position++;
                justCleanBuffer();
            } else if(isprint(c) || c == SPACE){
                addLetter(c);
                STATE = 28;
            }else if(c == EOF && last_char != 34){
                //mensagem de erro
                printf("Error. Você esqueceu um \" na linha %d.\n",line_number);
            }
            break;
        case 35:
            if(c == '='){
                addLetter(c);
                printToken(SN, c);
                justCleanBuffer();
            }else{
                printToken(SN, c);
                cleanBuffer(f, c);
            }
            break;
        case 38:
            if(c == '&'){
                addLetter(c);
                printToken(SN, c);
                justCleanBuffer();
            }else{
                //mensagem de erro
                printf("Error. Operador & não reconhecido na linha %d.\n", line_number);
            }
            break;
        case 39:
            if(c == '|'){
                addLetter(c);
                printToken(SN, c);
                justCleanBuffer();
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

void printToken(TokenType tp, char value){
    switch(tp){
        case ID:
             printf("<ID, %d>\n", indetifier_position);
        break;
        case PR:
            printf("<PR, %s>\n", reserved_word[isReservedWord(buffer)]);
        break;
        case CT_I:
            printf("<CT_I, %d>\n", getInteger());
        break;
        case CT_R:
            printf("<CT_R, %.2f>\n", getFloat());
        break;
        case CT_L:
            printf("<CT_L, %d>\n", literal_position);
        break;
        case COMMENT:
            printf("<COMMENT>\n");
        break;
        case SN:
            printf("<SN, %s>\n", signalsName[isSignal(buffer)]);
        break;
        case eOF:
            printf("<EOF, 0>\n");
        break;
        case CT_C:
            printf("<CT_C, %s>\n", buffer);
        break;
    }
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
