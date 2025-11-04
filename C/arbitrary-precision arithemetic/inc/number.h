#ifndef NUMBER_H_IS_INCLUDED
#define NUMBER_H_IS_INCLUDED

#include "file_handler.h"
#include <stddef.h>



/* * Liczba zapisana jest w zmiennej repr (representation). Program ma obs³ugiwaæ wszystkie systemy liczbowe o
 * podstawie [2, 16], wiêc do zapisania ka¿dej cyfry w dowolnym systemie potrzebne s¹ 4 bity
 * {1111(2) == 15(10) == F(16) — najwiêksza cyfra systemu szesnastkowego}. Na jednym bajcie (jednej zmiennej
 * typu char) zapisujê po dwie cyfry w celu oszczêdzania pamiêci operacyjnej. Liczby zapisane w repr s¹
 * zapisane w kodzie BCH (Binary-Coded Hexadecimal) w formacie: najmniej znacz¹ca cyfra jest na pierwszym
 * indeksie, natomiast najbardziej znacz¹ca na ostatnim. */

typedef struct {
	unsigned char* repr;	/* representation — reprezentacja liczby za pomoc¹ ³añcucha znaków */
	size_t repr_size;		/* pamiêci RAM wyra¿ona w bajtach zajmowana przez zmienn¹ repr */
	size_t length;			/* zmienna typu size_t dostosowuje siê do architektury urz¹dzenia, na którym uruchomiony jest program i zapewnia maksymaln¹ zgodnoœæ i zasiêg indeksowania po wszystkich obiektach */
	short base;				/* podstawa systemu liczbowego, w którym jest zapisana liczba */

} * number_t;


/* === DOSTÊPNE FUNKCJE === */
number_t num_init(short base);
number_t num_initWithSize(short base, size_t size);
number_t num_copyWithInit(number_t);
int num_expand(number_t);
void num_free(number_t*);

/* operacje na cyfrach tj. gettery i settery */
unsigned char num_getDigit(number_t, size_t index);
void num_setDigit(number_t, size_t index, unsigned char digit);
int num_addDigit(number_t, unsigned char digit);

int num_read(number_t, file_handler_t);
int num_reverse(number_t);
char* num_toString(number_t);
void num_delFrontZeros(number_t);

#endif /** NUMBER_H_IS_INCLUDED **/
