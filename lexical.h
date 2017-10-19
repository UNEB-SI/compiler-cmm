 enum token_types {ID, PR, SN, INTCON, REALCON, CADEIACON, CARACCON, eOF, COMMENT};
 typedef enum token_types TokenType;

//reserved word
char *reserved_word[] = {"inteiro", "real", "caracter", "booleano", "se", "senao", "semretorno", "enquanto", "para", "retorne", "semparam", "verdadeiro", "falso"};
//accept signals
char *signals[] = {">","<","<=", ">=", "!", "!=", ";",",", "&&","||","+","-","*","/", "(", ")", "{", "}", "=", "=="};
char *signalsName[] = {"MAIOR","MENOR","MENOR_QUE", "MAIOR_QUE", "NEG", "DIF", "PT_VIRG","VIRGULA", "E","OU","ADD","SUB","MULT","DIV", "PAREN_ABER", "PAREN_FEC", "CHAVE_ABER", "CHAVE_FEC", "ATRIBUICAO", "IGUALDADE"};
//accept constants
const char TAB = '\t';
const char ENTER = '\n';
const char SPACE = ' ';
const char UNDERLINE = '_';
const char BAR = '/';
const int INVERTED_BAR = 92;
const int APOSTROPHE = 39;
const int QUOTES = 34;
const char* ERROR_PASS_FILE = "Você deve indicar um arquivo para ser analisado. Ex: lexical namefile.cmm";
const char* ERROR_NOT_FOUND_FILE = "Arquivo não encontrado!";
const char* ERROR_NUMBER_FLOAT_FORMAT = "Esperado um número após ";
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
