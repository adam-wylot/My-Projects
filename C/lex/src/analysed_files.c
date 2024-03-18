// NAGŁÓWKI
#include "analysed_files.h"
#include "analyser.h"
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINICJE FUNKCJI
analysed_file_t init_file( char* file_path ) {
// alokacja pamięci
    analysed_file_t analysed_file = (analysed_file_t)malloc( sizeof *analysed_file );
    if( analysed_file == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się wczytać pliku \"%s\" - wystąpił błąd podczas alokowania pamięci [1]", file_path );
        push_error( str );

        return NULL;
    }

    analysed_file->fun_arr = (fun_t*)malloc( sizeof(fun_t) );
    if( analysed_file->fun_arr == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się wczytać pliku \"%s\" - wystąpił błąd podczas alokowania pamięci [2]", file_path );
        push_error( str );

        free( analysed_file );
        return NULL;
    }

    analysed_file->path = (char*)malloc( (strlen( file_path ) + 1) * sizeof(char) );
    if( analysed_file->path == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się wczytać pliku \"%s\" - wystąpił błąd podczas alokowania pamięci [3]", file_path );
        push_error( str );

        free( analysed_file->fun_arr );
        free( analysed_file );
        return NULL;
    }

    analysed_file->blacklist = init_blacklist( NULL );
    if( analysed_file->blacklist == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się wczytać czarnej listy dla pliku \"%s\" - wystąpił błąd podczas alokowania pamięci", file_path );
        push_crit_error( str );
    }

// wartości początkowe
    analysed_file->file = NULL;
    analysed_file->line = 0;
    analysed_file->arr_size = 0;
    analysed_file->arr_capacity = 1;
    strcpy( analysed_file->path, file_path );

    return analysed_file;
}

bool read_file( analysed_file_t analysed_file ) {
    if( analysed_file == NULL ) { // zabezpieczenie
        push_error ( "nie udało się przeczytać jednego z plików - utracono plik" );
        return EXIT_FAILURE;
    }

    // wypisanie informacji o pliku
    char str[BUFSIZE];
    sprintf( str, "czytam plik \"%s\"", analysed_file->path );
    send_message( str );

    // czytanie pliku
    if( analyse_file( analysed_file ) == EXIT_FAILURE ) {
        // wykonuje poniższe czynności przy niepowodzeniu
        sprintf( str, "zakończono czytanie pliku \"%s\" - wystąpił błąd", analysed_file->path  );
        push_error( str );

        fclose( analysed_file->file );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int is_file_exist( char* path ) {
    // iteracja po wszystkich wczytanych plikach
    for( unsigned i=0; i < repository.arr_size; ++i ) {
        if( strcmp( repository.files_arr[i]->path, path ) == 0 ) {
            return i;
        }
    }

    return -1;
}

void free_file( analysed_file_t analysed_file ) {
    for( unsigned i=0; i < analysed_file->arr_size; ++i ) {
        free_fun( analysed_file->fun_arr[i] );
    }
    free( analysed_file->fun_arr );

    free( analysed_file->path );
    free_blacklist( analysed_file->blacklist );
    free( analysed_file );
}


void add_fun( analysed_file_t analysed_file, char* fun_name ) {
    // powiększa zajmowaną pamięć (gdy jest taka potrzeba)
    if( analysed_file->arr_size >= analysed_file->arr_capacity ) {
        fun_t* tmp_ptr = (fun_t*)realloc( analysed_file->fun_arr, 2 * analysed_file->arr_capacity * sizeof(fun_t) );
        if( tmp_ptr == NULL ) { // zabezpieczenie
            char str[BUFSIZE];
            sprintf( str, "nie udalo się dodać funkcji \"%s\" - wystąpił błąd podczas realokowania pamięci tablicy", fun_name );
            push_error( str );
            return;
        }

        analysed_file->fun_arr = tmp_ptr;
        analysed_file->arr_capacity *= 2;
    }

    // dodanie funkcji na ostatnie miejsce
    analysed_file->fun_arr[ analysed_file->arr_size ] = init_fun( fun_name );
    ++(analysed_file->arr_size);
}
