#ifndef LEXICAL_H
#define LEXICAL_H
/*
  * Created by: José Diôgo
  * Last Modification: 13/12/2017
*/
#include <stdio.h>
#include "structures.h"

// It represents each type of token possible on CMM
enum token_types {ID, PR, SN, INTCON, REALCON, CADEIACON, CARACCON,
                  eOF, COMMENT};
typedef enum token_types token_type;

/**
* This struct holds and identifier token, with it's value and
* table position
*/
typedef struct {
  int table_position;
  char value[300];
} lexem;

/*
** Structure of a Token in CMM language
*/
typedef struct {
  token_type type; //holds the type (integer, character, real)
  union {
    char cValue; // caracter values
    float dValue; // float values
    int iValue; // integer values
    char pr[15]; // keeps reserved words
    char signal[2]; // keeps signal values(+, -, *, / e etc)
    char word[1000]; // literal values
    lexem lexem; // Identifiers
  };
} Token;

// One variable store actual token been processed, and next_token
// keeps the value of a token in sequence
Token token, next_token;

/**
* Receives a path for an file and read it char by char
*/
void readFile(char *file_name);
/**
* Receives a FILE object and verify for each AFD state, if
* the next token is valid for CMM.
*/
int checkState(FILE *f);
/**
* Receives a letter and return 1 if it is a alpha char and 0 otherwise
*
*/
int isLetter(char letter);
int isReservedWord(char *word); //check if a string is a reserved word
int isSignal(char *word); //check if a string is a signal like +, - , / and etc
void addLetter(char c); // add actual char to buffer and keeps its value on last char
void justCleanBuffer(); // Erase buffer and starts and keeps from next token
void addStringFinal(); //add /0 to buffer array
void printToken(token_type tp, char value); //print a specific token
void cleanBuffer(FILE *f, char c); // Erase buffer and unget actual char
int getInteger(); // Transform a char array into Int
float getFloat(); // Transform a char array into Float
Token getToken(); // Retorn actual Token
int isFirstTime(); // Check if it is first time passing through code
#endif // LEXICAL
