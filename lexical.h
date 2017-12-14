#ifndef LEXICAL_H
#define LEXICAL_H
/*
  * Created by: José Diôgo
  * Last Modification: 13/12/2017
*/
#include <time.h>
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
  TokenType type;
  union {
    char cValue;
    float dValue;
    int iValue;
    char pr[15];
    char signal[2];
    char word[1000];
    LEXEM lexem;
  };
} Token;

Token token, next_token;

//tables
char identifiers[300][300];
char literals[300][300];

int first_time;

//keep the name of signals
enum signals_name {MAIOR = 0, MENOR, MENOR_IGUAL, MAIOR_IGUAL, NEG, DIF, PT_VIRG, VIRGULA, E, OU, ADD, SUB, MULTI, DIV} SignalName;

//functions definition
void readFile(char *file);
void errorMessage(const char *error);
int checkState(FILE *f);
int isLetter(char letter);
int isReservedWord(char *word);
int isSignal(char *word);
void addLetter(char c);
void justCleanBuffer();
void addStringFinal();
void printToken(TokenType tp, char value);
void cleanBuffer(FILE *f, char c);
int getInteger();
float getFloat();
Token getToken();
int isFirstTime();
void getLoadOrPush(Token t);
void operator_check(Token t);
#endif // LEXICAL
