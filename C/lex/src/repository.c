//                  NAGŁÓWKI
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//                  DEFINICJE FUNKCJI
void init_rep( int argc, char** argv ) {
// PODSTAWOWE ZMIENNE
    repository.program_name = argv[0];
    repository.global_blacklist = init_blacklist( "blacklist" );
    repository.argc = argc;
    repository.argv = argv;

// TABLICA PLIKÓW
    // alokowanie pamięci
    repository.files_arr = (analysed_file_t*)malloc( sizeof(analysed_file_t) );
    if( repository.files_arr == NULL ) {
        push_crit_error( "" );
        fprintf( stderr, "%s: błąd: nie udało się wczytać żadnych plików - wystąpił błąd podczas alokowania pamięci!\n", repository.program_name );
        fprintf( stderr, "%s: błąd krytyczny - kończę działanie programu.\n", repository.program_name );

        free_blacklist( repository.global_blacklist );
        exit( EXIT_FAILURE );
    }
    // ustawienie wartości początkowych tablicy plików
    repository.arr_size = 0;
    repository.arr_capacity = 1;

    // inicjalizacja tablicy plików
    for( int i=1; i < argc; ++i ) {
        (void) load_file( argv[i] );
    }
}

void print_all( void ) {
    // iterowanie po tablicy plików
    for( unsigned i=0; i < repository.arr_size; ++i ) {
        // iterowanie po tablicy funkcji w pojedyńczym pliku
        for( unsigned j=0; j < repository.files_arr[i]->arr_size; ++j ) {
            print_fun( repository.files_arr[i]->fun_arr[j] );
        }
    }
}

void free_all( void ) {
    // czyszczenie tablicy funkcji w każdym z plików, a następnie samej tablicy
    for( unsigned i=0; i < repository.arr_size; ++i ) {
        if( repository.files_arr[i] != NULL ) { free_file( repository.files_arr[i] ); }
    }
    free( repository.files_arr );

    // czyszczenie globalnej czarnej listy (lokalne czyszczone są w free_file )
    free_blacklist( repository.global_blacklist );
}

// Operacje na tablicy
int load_file( char* path ) {
    // zabezpieczenie przed dublowaniem plików
    if( is_file_exist( path ) != -1 ) {
        return -1;
    }

    // inicjowanie pliku, który chcemy wczytać
    analysed_file_t tmp_var = init_file( path );
    if( tmp_var == NULL ) { // wystąpił jakiś błąd
        return -1;
    }

    // dodajemy wczytany plik do tablicy i pobieramy jego indeks (indeks ostatniego elementu w tablicy)
    add_file( tmp_var );
    int index = (int) repository.arr_size - 1;
    // czytanie wczytanego pliku
    if( read_file( tmp_var ) == EXIT_FAILURE ) { // wystąpił błąd podczas czytania pliku
        remove_file( index );
        return -1;
    }

    // jeżeli sukces to zwraca indeks w tablicy plików wczytanego pliku
    return index;
}

void add_file( analysed_file_t analysed_file ) {
    // realokowanie pamięci (gdy jest taka potrzeba)
    if( repository.arr_size >= repository.arr_capacity ) { // jest za mało pamięci
        analysed_file_t* new_arr = (analysed_file_t*) realloc( repository.files_arr, 2 * repository.arr_capacity * sizeof(analysed_file_t) );
        if( new_arr == NULL ) { // zabezpieczenie
            char tmp_str[BUFSIZE];
            sprintf( tmp_str, "nie udało się dodać pliku \"%s\" do tablicy z plikami - wystąpił błąd podczas realokowania pamięci", analysed_file->path );
            push_crit_error( tmp_str );
        }

        repository.files_arr = new_arr;
        repository.arr_capacity *= 2;
    }


    // dodanie elementu do tablicy
    repository.files_arr[ repository.arr_size ] = analysed_file;
    ++(repository.arr_size);
}

void remove_file( unsigned index ) {
    if( repository.arr_size <= index ) { // zabezpieczenie
        return;
    }

    // usuwanie elementu
    free_file( repository.files_arr[ index ] );
    --( repository.arr_size );

    // przesuwanie następnych element o jeden do tyłu
    for( unsigned i = index; i < repository.arr_size; ++i ) {
        repository.files_arr[ i ] = repository.files_arr[ i+1 ];
        repository.files_arr[ i+1 ] = NULL;
    }
}



// Wiadomości | Błędy
void push_error( char* message ) {
    fprintf( stderr, "%s: błąd: %s!\n", repository.program_name, message );
}

void push_crit_error( char* message ) {
    push_error( message );
    fprintf( stderr, "%s: błąd krytyczny - kończę działanie programu.\n", repository.program_name );

    free_all();
    exit( EXIT_FAILURE );
}

void send_message( char* message ) {
    printf( "%s: %s.\n", repository.program_name, message );
}
