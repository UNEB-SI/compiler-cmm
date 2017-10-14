 enum token_types {ID, PR, SN, CT_I, CT_R, LT, CT_C, eOF, COMMENT};
 typedef enum token_types TokenType;
//defining a token
typedef struct{
    TokenType tp;
    char *valor;
} Token;

//reserved word
char *reserved_word[] = {"inteiro", "real", "caracter", "booleano", "se", "senao", "sem retorno", "enquanto", "para", "retorne"};
//accept signals
char *signals[] = {">","<","<=", ">=", "!", "!=", ";",",", "&&","||","+","-","*","/", "[", "]", "(", ")", "{", "}", "=", "=="};
char *signalsName[] = {"MAIOR","MENOR","MENOR_QUE", "MAIOR_QUE", "NEG", "DIF", "PT_VIRG","VIRGULA", "E","OU","ADD","SUB","MULT","DIV", "COL_ABER", "COL_FEC", "PAREN_ABER", "PAREN_FEC", "CHAVE_ABER", "CHAVE_FEC", "ATRIBUICAO", "IGUALDADE"};
//accept constants
const char TAB = '\t';
const char ENTER = '\n';
const char SPACE = ' ';
const char UNDERLINE = '_';
const char* ERROR_PASS_FILE = "Você deve indicar um arquivo para ser analisado. Ex: lexical namefile.cmm";
const char* ERROR_NOT_FOUND_FILE = "Arquivo não encontrado!";
const char* ERROR_NUMBER_FLOAT_FORMAT = "Esperado um número após ";
//tables
char identifiers[50];

//keep the name of signals
enum signals_name {MAIOR = 0, MENOR, MENOR_IGUAL, MAIOR_IGUAL, NEG, DIF, PT_VIRG, VIRGULA, E, OU, ADD, SUB, MULTI, DIV} SignalName;

//functions definition
void readFile(char *file);
void errorMessage(const char *error);
void checkState(char c, FILE *f);
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
