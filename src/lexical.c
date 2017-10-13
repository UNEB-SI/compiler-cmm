#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../include/lexical.h"

char actual_char;
int STATE = 0;
char buffer[50];
int buffer_position = 0;
int indetifier_position = 0;

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
            checkState(actual_char, file);
        }
        checkState(actual_char, file);
    }else{
        errorMessage(ERROR_NOT_FOUND_FILE);
    }
}

void checkState(char c, FILE *f){
    switch(STATE){
        case 0:
            if(isLetter(c)){
                STATE = 1;
                buffer[buffer_position] = c;
                buffer_position++;
            }else if(isdigit(c)){
                STATE = 3;
                buffer[buffer_position] = c;
                buffer_position++;
            }else if(c == ' ' || c == '\t' || c == '\n'){
                STATE = 0;
            }
            break;
        case 1:
            if(isLetter(c) || isdigit(c) || c == '_'){
                STATE = 1;
                buffer[buffer_position] = c;
                buffer_position++;
            }else{
                buffer[buffer_position] = '\0';
                buffer_position++;
                int is_reserved_word = 0;
                if((is_reserved_word = isReservedWord(buffer)) != -1){
                    printf("<PR, %d>\n", is_reserved_word);
                }else{
                    strcpy(&identifiers[indetifier_position], buffer);
                    printf("<ID, %d>\n", indetifier_position);
                    indetifier_position++;                    
                }
                
                buffer_position = 0;
                //clean buffer
                memset(&buffer[0], 0, sizeof(buffer));
                STATE = 0;
                if(c != EOF){
                    ungetc(c, f);
                }
            }
            break;
        case 3:
            if(isdigit(c)){
                STATE = 3;
                buffer[buffer_position] = c;
                buffer_position++;
            }else if(c == '.'){
                STATE = 5;
                buffer[buffer_position] = c;
                buffer_position++;
            }else{
                buffer[buffer_position] = '\0';
                buffer_position++;
                buffer_position = 0;
                int number = (int) strtol(buffer, (char **)NULL, 10);
                printf("<CT_I, %d>\n", number);
                //clean buffer
                memset(&buffer[0], 0, sizeof(buffer));
                STATE = 0;
                if(c != EOF){
                    ungetc(c, f);
                }
            }
            break;
        case 5:
            if(isdigit(c)){
                STATE = 6;
                ungetc(c, f);
            }else{
                //given an error
            }
            break;
        case 6:
            if(isdigit(c)){
                STATE = 6;
                buffer[buffer_position] = c;
                buffer_position++;
            }else{
                buffer[buffer_position] = '\0';
                buffer_position++;
                buffer_position = 0;
                float number = atof(buffer);
                printf("<CT_R, %.2f>\n", number);
                //clean buffer
                memset(&buffer[0], 0, sizeof(buffer));
                STATE = 0;
                if(c != EOF){
                    ungetc(c, f);
                }
            }
            break;
    }
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

void errorMessage(const char *error){
    printf("Error: %s\n", error);
}