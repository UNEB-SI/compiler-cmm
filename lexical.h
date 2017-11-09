#ifndef LEXICAL_H
#define LEXICAL_H

 enum token_types {ID, PR, SN, INTCON, REALCON, CADEIACON, CARACCON, eOF, COMMENT};
 typedef enum token_types TokenType;

typedef struct {
  int table_position;
  char value[300];
} LEXEM;

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

//keep the name of signals
enum signals_name {MAIOR = 0, MENOR, MENOR_IGUAL, MAIOR_IGUAL, NEG, DIF, PT_VIRG, VIRGULA, E, OU, ADD, SUB, MULTI, DIV} SignalName;

//functions definition
void readFile(char *file);
void errorMessage(const char *error);
int checkState(char c, FILE *f);
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

#endif // LEXICAL
