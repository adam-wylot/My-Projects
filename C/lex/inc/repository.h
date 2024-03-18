#ifndef _REPOSITORY_H_IS_INCLUDED_
#define _REPOSITORY_H_IS_INCLUDED_

// NAGŁÓWKI
#include <stdbool.h>

#include "fun.h"
#include "blacklist.h"
#include "analysed_files.h"

// STAŁE
#define BUFSIZE 512

// OBIEKT STRUKTURY
/* repository.h (repozytorium) jest niejakim magazynem funkcji i zmiennych, żeby pozostawić plik main.c w jak najczystszej formie */
struct {
    char* program_name; // inaczej mówiąc argv[0] - przechowuje ścieżkę wywołania programu
    int argc;
    char** argv;

    blacklist_t global_blacklist; // globalna czarna lista nazw funkcji - działa we wszystkich plikach

    // tablica plików - każdy element przechowuje informacje o jednym z czytanych plików
    analysed_file_t* files_arr;
    unsigned arr_size;
    unsigned arr_capacity;

} repository;


// PROTOTYPY POWIĄZANYCH FUNKCJI
void init_rep( int, char** );       // inicjuje jedyny istniejący w całym programie plik repozytorium
void print_all( void );             // rysuje wszystkie informacje o wszystkich funkcjach, jakie udało się zebrać programowi
void free_all( void );              // zwalnia całą zajętą przez program pamięć

    // Operacje na tablicy
int load_file( char* );             // load_file( ścieżka do pliku ) - rozpoczyna wczytywanie pliku; zwraca indeks zajmowanego miejsca w tabeli przez plik, gdy sukces; zwraca -1, gdy porażka
void add_file( analysed_file_t );   // dodaje plik do tablicy plików
void remove_file( unsigned );       // usuwa plik z tablicy plików

    // wiadomości
void push_error( char* );           // wyrzuca błąd w wyjściu stderr
void push_crit_error( char* );      // wyrzuca błąd w wyjściu stderr i kończy działanie programu
void send_message( char* );         // wypisuje informację w wyjściu stdout

#endif // _REPOSITORY_H_IS_INCLUDED_
