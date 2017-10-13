 enum token_types {ID, PR, SN, CT_I, CT_R, LT, CT_C, eOF};
 typedef enum token_types TokenType;
//defining a token
typedef struct{
    TokenType tp;
    char *valor;
} Token;

//reserved word
char *reserved_word[] = {"inteiro", "real", "caracter", "booleano", "se", "senao", "sem retorno", "enquanto", "para", "retorne"};
//accept signals
char *signals[] = {">","<","<=", ">=", "!", "!=", ";",",", "&&","||","+","-","*","/"};
//accept constants
const char* TAB = "\t";
const char* ENTER = "\n";
const char* SPACE = " ";
const char* ERROR_PASS_FILE = "Você deve indicar um arquivo para ser analisado. Ex: lexical namefile.cmm";
const char* ERROR_NOT_FOUND_FILE = "Arquivo não encontrado!";
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
