#ifndef _ANALYSED_FILES_H_IS_INCLUDED_
#define _ANALYSED_FILES_H_IS_INCLUDED_

// NAGŁÓWKI
#include <stdio.h>
#include <stdbool.h>

#include "fun.h"
#include "blacklist.h"

// STRUKTURA
typedef struct {
    FILE* file; // wskaźnik na plik
    char* path; // ścieżka do pliku
    unsigned line; // aktualna linia | liczba linii w pliku
    blacklist_t blacklist; // lokalna czarna lista nazw funkcji

    // tablica funkcji, które znajdują się w tym pliku, oraz powiązane z nią zmienne
    fun_t* fun_arr;
    unsigned arr_size;
    unsigned arr_capacity;
} * analysed_file_t;

// PROTOTYPY POWIĄZANYCH FUNKCJI
analysed_file_t init_file( char* );         // inicjalizacja pliku o podanej ścieżce
bool read_file( analysed_file_t );          // czyta plik wprowadzony w ścieżce przy inicjalizacji; zwraca SUKCES lub PORAŻKĘ
int is_file_exist( char* );                 // sprawdza, czy istnieje jakiś plik z podaną ścieżką
void free_file( analysed_file_t );          // usunięcie informacji o pliku i zajmowanych przez te informacje miejsca

    // operacje na funkcjach
void add_fun( analysed_file_t, char* );     // dodaje funkcję do tablicy funkcji w pliku


#endif // _ANALYSED_FILES_H_IS_INCLUDED_
