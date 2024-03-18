#ifndef _FUN_STACK_H_IS_INCLUDED_
#define _FUN_STACK_H_IS_INCLUDED_

// NAGŁÓWKI
#include <stdbool.h>

// STRUKTURA
typedef struct f_s {
    int line;               // linia w pliku, w której znajduje się nazwa funkcji
    char* fun_name;         // nazwa funkcji

    struct f_s* previous;   // wskaźnik na poprzedni element na stosie
} * fun_stack_t;

// PROTOTYPY POWIĄZANYCH FUNKCJI
int fun_stack_line_value( fun_stack_t );                /* zwraca line - linijkę wystąpienia funkcji przechowywane na szczycie */
void fun_stack_push( fun_stack_t*, char*, unsigned );   /* odkłada na stos parę (line, fun_name) */
char* fun_stack_get( fun_stack_t* );                    /* usuwa z wierzchołka parę (line, fun_name), zwraca zdjętą fun_name */
char* fun_stack_seek( fun_stack_t );                    /* podgląda nazwę funkcji ze szczytu stosu */
bool fun_stack_is_empty( fun_stack_t );                 /* zwraca informację czy stos jest pusty */
void fun_stack_clear( fun_stack_t* );                   /* czyści cały stos - usuwanie wszystkich elementów */

#endif // _FUN_STACK_H_IS_INCLUDED_
