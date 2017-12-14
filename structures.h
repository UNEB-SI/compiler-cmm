/*A Pair value to store the type and value of a expression*/
typedef struct{
  char type[10];
  union {
    char cValue;
    float dValue;
    int iValue;
    int bValue;
    char signal[2];
    char word[1000];
  };
} expression;
