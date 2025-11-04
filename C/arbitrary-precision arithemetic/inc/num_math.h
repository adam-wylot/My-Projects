#ifndef NUM_MATH_H_IS_INCLUDED
#define NUM_MATH_H_IS_INCLUDED

#include "number.h"
#include "bool.h"



/* === OPERACJE Z£O¯ONE === */
number_t add(number_t, number_t);
number_t subtract(number_t, number_t);
number_t multiply(number_t, number_t);
number_t divide(number_t, number_t);
number_t expand(number_t, number_t);
number_t modulo(number_t, number_t);
number_t decToAny(number_t, short base);
number_t anyToDec(number_t);
number_t anyToAny(number_t, short);


/* === OPERACJE PROSTE === */
int compare(number_t, number_t);
int simpleDiv(number_t, number_t);                      /* Proste dzielenie oparte o odejmowanie */
number_t multByConst(number_t, unsigned char value);    /* mno¿y podan¹ liczbê przez jak¹œ sta³¹ i tworzy now¹ liczbê wynikow¹ */
void addConst(number_t, unsigned char value);           /* dodaje sta³¹ do podanej liczby */
int numToInt(number_t);                                 /* je¿eli liczba jest w zakresie to konwertuje j¹ na liczbê typu int */
number_t intToNum(int value, short base);               /* konwersja liczby typu int na liczbê d³ug¹ */


/* === OPERACJE TRUE-FALSE === */
bool isZero(number_t);
bool isHexNumber(char*);    /* Sprawdza, czy podany ³añcuch znaków jest liczb¹ heksadecymaln¹. */
bool isDecDigit(char);      /* Sprawdza, czy podany znak jest cyfr¹ w systemie dziesiêtnym. */
bool isHexDigit(char);      /* Sprawdza, czy podany znak jest cyfr¹ w systemie szesnastkowym. */


#endif /** NUM_MATH_H_IS_INCLUDED **/