// NAGŁÓWKI
#include "blacklist.h"
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINICJE FUNKCJI
blacklist_t init_blacklist( char* file_path ) {
// alokacja pamięci
    blacklist_t blacklist = (blacklist_t)malloc( sizeof *blacklist );
    if( blacklist == NULL ) {
        push_crit_error( "nie udało się zainicjować czarnej listy nazw funkcji - wystąpił błąd podczas alokowania pamięci" );
    }

    blacklist->array = (char**)malloc( sizeof(char*) );
    if( blacklist->array == NULL ) {
        push_crit_error( "nie udało się zainicjować czarnej listy nazw funkcji - wystąpił błąd podczas alokowania pamięci" );
    }
    blacklist->size = 0;
    blacklist->capacity = 1;

// sprawdzanie, czy należy pobrać nazwy funkcji z pliku, czy jednak załączonego pliku nie ma
    if( file_path == NULL ) {
        return blacklist;
    }


// wczytywanie nazw funkcji do czarnej listy
    FILE* file = fopen( file_path, "r" );
    if( file == NULL ) { // zabezpieczenie
        push_error( "nie udało się zainicjować czarnej listy nazw funkcji - wystąpił błąd podczas otwierania pliku z nazwami" );
        return blacklist;
    }

    // czytanie pliku
    char str[BUFSIZE];

    while( fscanf( file, "%s", str ) != EOF ) {
        if( add_to_blacklist( blacklist, str ) == EXIT_FAILURE ) { // zabezpieczenie
            push_error( "nie udało się w pełni zainicjować czarnej listy nazw funkcji - wystąpił błąd podczas wczytywania nazw funkcji" );
            fclose( file );

            // usuwanie wczytanych do tej pory elementów
            for( unsigned i=0; i < blacklist->size; ++i ) {
                free( blacklist->array[i] );
            }
            blacklist->size = 0;

            return blacklist;
        }
    }

    fclose( file );
    return blacklist;
}

bool blacklist_copy( blacklist_t dest, blacklist_t source ) {
    if( dest == NULL || source == NULL ) { // zabezpieczenie
        push_error( "nie udało się zainicjować czarnej listy nazw funkcji - wystąpił błąd podczas kopiowania czarnej listy" );
        return EXIT_FAILURE;
    }

    // Kopiowanie elementów
    for( unsigned i=0; i < source->size; ++i ) {
        add_to_blacklist( dest, source->array[i] );
    }

    return EXIT_SUCCESS;
}

bool add_to_blacklist( blacklist_t blacklist, char* str ) {
    if( blacklist == NULL ) { // zabezpieczenie
        push_error( "nie udało się dodać funkcji do czarnej listy - wystąpił nieznany błąd" );
        return EXIT_FAILURE;
    }

    // powiększenie tablicy (jeżeli jest taka potrzeba)
    if( blacklist->size >= blacklist->capacity ) {
        char** new = (char**)realloc( blacklist->array, 2 * blacklist->capacity * sizeof(char*) );
        if( new == NULL ) { // zabezpieczenie
            push_error( "nie udało się dodać elementu do czarnej listy - wystąpił błąd podczas realokowania pamięci tablicy" );
            
            free_blacklist( blacklist ); // zwalnianie zajętej już pamięci
            return EXIT_FAILURE;
        }

        blacklist->array = new;
        blacklist->capacity *= 2;
    }

    // dodawanie elementu
    blacklist->array[ blacklist->size ] = (char*)malloc( ( strlen( str ) + 1 ) * sizeof(char) );
    if( blacklist->array[ blacklist->size ] == NULL ) { // zabezpieczenie
        push_error( "nie udało się dodać elementu do czarnej listy - wystąpił błąd podczas alokowania pamięci elementu" );

        free_blacklist( blacklist ); // zwalnianie zajętej już pamięci
        return EXIT_FAILURE;
    }

// ustawienie wartości
    strcpy( blacklist->array[ blacklist->size ], str );
    ++(blacklist->size);

    return EXIT_SUCCESS;
}

void free_blacklist( blacklist_t blacklist ) {
    if( blacklist == NULL ) { // zabezpieczenie
        return;
    }

    // czyszczenie pojedyńczych elementów tablicy i czyszczenie tablicy
    for( unsigned i=0; i < blacklist->size; ++i ) {
        free( blacklist->array[i] );
    }
    free( blacklist->array );

    free( blacklist );
}

bool check_in_blacklist( blacklist_t blacklist, char* str ) {
    // czarna lista globalna
    for( unsigned i=0; i < repository.global_blacklist->size; ++i ) {
        if( strcmp( repository.global_blacklist->array[i], str ) == 0 ) {
            return true;
        }
    }

    if( blacklist == NULL ) { // zabezpieczenie
        return false;
    }

    // czarna lista lokalna
    for( unsigned i=0; i < blacklist->size; ++i ) {
        if( strcmp( blacklist->array[i], str ) == 0 ) {
            return true;
        }
    }

    return false;
}
