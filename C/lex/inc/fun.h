#ifndef _FUN_H_IS_INCLUDED_
#define _FUN_H_IS_INCLUDED_

// NAGŁÓWKI
#include "occurrence.h"
#include "fun_dictionary.h"

// STRUKTURA
typedef struct {
    char* name; // nazwa funkcji
    occurrence_t definition;    // definicja funkcji
    occurrence_t prototype;     // prototyp funkcji
    fun_dictionary_t calls;     // słownik z funkcjami, które wywołuje ta funkcja

    // tablica użyć funkcji i jej zmienne
    occurrence_t* usages;
    unsigned usages_size;
    unsigned usages_capacity;
} * fun_t;


// PROTOTYPY POWIĄZANYCH FUNKCJI
fun_t init_fun( char* );                                // inicjuje funkcję o podanej nazwie
fun_t get_fun( char* );                                 // zwraca funkcję (z listy wszystkich wczytanych do tej pory) o podanej nazwie
//void remove_fun( analysed_file_t, char* );            // usuwa funkcję (z listy wszystkich wczytanych do tej pory) o podanej nazwie
bool check_in_fun_arr( char* );                         // sprawdza, czy funkcja o podanej nazwie została już zadeklarowana
void print_fun( fun_t );                                // wypisuje w stdout wszystkie informacje o podanej funkcji
void free_fun( fun_t );                                 // zwalnia zajmowaną przez podaną funkcję pamięć

    // działania na poszczególnych wystąpieniach funkcji
void add_def( fun_t, char*, unsigned, unsigned );       // dodaje definicje oraz miejsce jej występowania do podanej funkcji
void add_prot( fun_t, char*, unsigned, unsigned );      // dodaje prototyp oraz miejsce jego występowania do podanej funkcji
void add_usage( fun_t, char*, unsigned, unsigned );     // dodaje użycie tej funkcji oraz miejsce jego występowania
void add_call( fun_t, char* );                          // dodaje wywołanie innej funkcji w środku deklaracji tej funkcji

#endif // _FUN_H_IS_INCLUDED_
