#ifndef STRUCTURES_H
#define STRUCTURES_H
/*
  * Created by: José Diôgo
  * Last Modification: 13/12/2017
*/

/*
** A Pair value to store the type and value of an expression
*/
typedef struct{
  char type[10]; //holds the type (integer, character, real, boolean)
  union {
    char cValue; // caracter values
    float dValue; // float values
    int iValue; // integer values
    int bValue; // boolean values
    char word[1000]; // literal values
  };
} expression;

#endif // STRUCTURES
