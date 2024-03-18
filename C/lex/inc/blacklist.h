#ifndef _BLACKLIST_H_IS_INCLUDED_
#define _BLACKLIST_H_IS_INCLUDED_

// NAGŁÓWKK
#include <stdbool.h>

// STRUKTURA
typedef struct {

    // tablica słów i jej zmienne
    char** array;
    unsigned size;
    unsigned capacity;
} * blacklist_t;

// PROTOTYPY POWIĄZANYCH FUNKCJI
blacklist_t init_blacklist( char* );                // inicjowanie czarnej listy słów
bool blacklist_copy( blacklist_t, blacklist_t );    // kopiowanie elementów z jednej listy do drugiej
void free_blacklist( blacklist_t );                 // usuwa wszystkie elementy tablicy i czyści zajmowaną przez nie pamięć

bool add_to_blacklist( blacklist_t, char* );        // dodanie słowa (nazwy funkcji) do czarnej listy
bool check_in_blacklist( blacklist_t, char* );      // sprawdza czy podane słowo (nazwa funkcji) znajduje się na czarnej liscie

#endif // _BLACKLIST_H_IS_INCLUDED_
