#ifndef _CELL_STACK_H_
#define _CELL_STACK_H_

                    /* === NAGŁÓWKI === */
#include <stdbool.h>

#include "cell.h"

                    /* === STRUKTURA STOSU === */
/* Służy do przechowywania komórek */
typedef struct st {
    cell_t cell;
    struct st* prev_p; /* wskaźnik na poprzedni element na stosie */
} * cstack_t;

                    /* === FUNKCJE === */
cell_t pop( cstack_t* );        /* pobiera element ze stosu i usuwa go      */
cell_t top( cstack_t* );        /* podgląda element na samej górze          */
void push( cstack_t*, cell_t ); /* wrzuca element na stos                   */
bool is_empty( cstack_t );      /* zwraca informację czy stos jest pusty    */
void clear_stack( cstack_t* );  /* usuwa wszystkie elementy ze stosu        */

#endif
