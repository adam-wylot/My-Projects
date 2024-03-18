// NAGŁÓWKI
#include "fun.h"
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINICJE FUNKCJI
fun_t init_fun( char* fun_name ) {

// alokowanie wymaganej pamięci
    fun_t fun = (fun_t)malloc( sizeof *fun );
    if( fun == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się zainicjować funkcji \"%s\"", fun_name );
        push_error( str );

        return NULL;
    }

    fun -> usages = (occurrence_t*)malloc( sizeof(occurrence_t) );
    if( fun -> usages == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się zainicjować funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun_name );
        push_error( str );

        // zwalnianie zajętej do tej pory pamięci
        free( fun );
        return NULL;
    }

    fun -> name = (char*)malloc( ( strlen(fun_name) + 1 ) * sizeof(char) );
    if( fun -> name == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się zainicjować funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun_name );
        push_error( str );

        // zwalnianie zajętej do tej pory pamięci
        free( fun -> usages );
        free( fun );
        return NULL;
    }

// przypisanie wartości początkowych
    strcpy( fun -> name, fun_name );
    fun -> definition = NULL;
    fun -> prototype = NULL;
    fun -> calls = NULL;
    fun ->  usages_capacity = 1;
    fun ->  usages_size = 0;

    return fun;
}

void print_fun( fun_t fun ) {
    printf( "Funkcja \"%s\":\n", fun->name );

// Rysowanie prototypu
    printf( "%*sPrototyp:\n", 3, "" );
    if( fun->prototype == NULL ) {
        printf( "%*s(brak)\n", 6, "" );
    }
    else {
        printf( "%*s", 6, "" );
        occ_print( fun->prototype );
    }

// Rysowanie dyfinicji
    printf( "%*sDefinicja:\n", 3, "" );
    if( fun->definition == NULL ) {
        printf( "%*s(brak)\n", 6, "" );
    }
    else {
        printf( "%*s", 6, "" );
        occ_print( fun->definition );
    }

// Rysowanie użyć
    printf( "%*sUżycie:\n", 3, "" );
    if( fun->usages_size == 0 ) {
        printf( "%*s(brak)\n", 6, "" );
    }
    else {
        for( unsigned i=0; i < fun->usages_size; ++i ) {
            printf( "%*s", 6, "" );
            occ_print( fun->usages[i] );
        }
    }

// Rysowanie wywołań
    printf( "%*sWywołuje:\n", 3, "" );
    if( fun->calls == NULL ) {
        printf( "%*s(brak)\n", 6, "" );
    }
    else {
        dict_print_all( fun->calls, 2 );
    }

    printf( "\n\n" );
}

void free_fun( fun_t fun ) {
// usuwanie definicji
    if( fun -> definition != NULL ) {
        free( fun -> definition -> file_name );
        free( fun -> definition );
    }

// usuwanie prototypu
    if( fun -> prototype != NULL ) {
        free( fun -> prototype -> file_name );
        free( fun -> prototype );
    }

// usuwanie użyć
    for( unsigned i=0; i < fun->usages_size; ++i ) {
        free( fun -> usages[i] -> file_name );
        free( fun -> usages[i] );
    }
    free( fun -> usages );

// zwolnienie słownika wywołań
    dict_free( &( fun -> calls ) );

    free( fun -> name );
    free( fun );
}

void add_def( fun_t fun, char* file_name, unsigned start, unsigned end ) {
// alokowanie pamięci
    if( fun -> definition != NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać definicji do funkcji \"%s\" - definicja już istnieje w pliku \"%s\" (początek: %d, koniec: %d)", fun->name, fun->definition->file_name, fun->definition->start, fun->definition->end );
        push_error( str );

        return;
    }

    fun -> definition = (occurrence_t)malloc(sizeof *(fun -> definition) );
    if( fun -> definition == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać definicji do funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );

        push_error( str );
        return;
    }

    fun->definition->file_name = (char*)malloc( ( strlen( file_name ) + 1 ) * sizeof(char) );
    if( fun->definition->file_name ==  NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać definicji do funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );

        free( fun->definition );
        push_error( str );
        return;
    }

// Przypisanie wartości początkowych
    fun -> definition -> type = definition;
    fun -> definition -> start = start;
    fun -> definition -> end = end;
    strcpy( fun->definition->file_name, file_name );
}

void add_prot( fun_t fun, char* file_name, unsigned start, unsigned end ) {
// alokowanie pamięci
    if( fun == NULL ) {
        push_error( "nie udało się dodać prototypu do funkcji - nie ma takiej funkcji" );
        return;
    }

    if( fun -> prototype != NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać prototypu do funkcji \"%s\" - prototyp już istnieje w pliku \"%s\" (początek: %d, koniec: %d)", fun->name, fun->prototype->file_name, fun->prototype->start, fun->prototype->end );
        push_error( str );

        return;
    }

    fun -> prototype = (occurrence_t)malloc(sizeof *(fun -> prototype) );
    if( fun -> prototype == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać prototypu do funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );
        push_error( str );

        return;
    }

    fun->prototype->file_name = (char*)malloc( ( strlen( file_name ) + 1 ) * sizeof(char) );
    if( fun->prototype->file_name ==  NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać prototypu do funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );
        push_error( str );

        free( fun->prototype );
        return;
    }

// Przypisanie wartości początkowych
    fun -> prototype -> type = prototype;
    fun -> prototype -> start = start;
    fun -> prototype -> end = end;
    strcpy( fun->prototype->file_name, file_name );
}

void add_usage( fun_t fun, char* file_name, unsigned start, unsigned end ) {
// sprawdza, czy jest miejsce w tablicy - jeżeli nie to zwiększa jej wielkość
    if( fun->usages_size >= fun->usages_capacity ) {
        occurrence_t* tmp_ptr = (occurrence_t*)realloc(fun->usages, 2 * fun->usages_capacity * sizeof(occurrence_t) );
        if( tmp_ptr == NULL ) { // zabezpieczenie
            char str[BUFSIZE];
            sprintf( str, "nie udało się dodać użycia funkcji \"%s\" - wystąpił błąd podczas realokowania pamięci tablicy", fun->name );
            push_error( str );
            return;
        }

        fun->usages = tmp_ptr;
        fun->usages_capacity *= 2;
    }

// dodawanie użycia funkcji
    fun->usages[ fun->usages_size ] = (occurrence_t)malloc(sizeof **(fun->usages) );
    if( fun->usages[ fun->usages_size ] == 0 ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać użycia funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );

        push_error( str );
        return;
    }

    fun->usages[ fun->usages_size ]->file_name = (char*)malloc( ( strlen( file_name ) + 1 ) * sizeof(char) );
    if( fun->usages[ fun->usages_size ]->file_name ==  NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać definicji do funkcji \"%s\" - wystąpił błąd podczas alokowania pamięci", fun->name );

        free( fun->usages[ fun->usages_size ] );
        push_error( str );
        return;
    }

// Przypisanie wartości początkowych
    strcpy( fun->usages[ fun->usages_size ]->file_name, file_name  );
    fun -> usages[ fun->usages_size ] -> type = usage;
    fun -> usages[ fun->usages_size ] -> start = start;
    fun -> usages[ fun->usages_size ] -> end = end;
    ++(fun->usages_size);
}

void add_call( fun_t fun, char* called_fun_name ) {
    // jeżeli jeszcze nie było użycia tej funkcji to dodaje ją do listy
    if( !dict_exist( fun->calls, called_fun_name ) ) {
        dict_add( &(fun->calls), called_fun_name, 1 );
        return;
    }

    // w przeciwnym wypadku dodaje jeden do wartości
    int value = dict_get_value( fun->calls, called_fun_name );
    ++value;

    dict_set_value( fun->calls, called_fun_name, value );
}




fun_t get_fun( char* fun_name ) {
    // iteruje po wczytanych plikach
    for( unsigned i=0; i < repository.arr_size; ++i ) {
        // iterowanie po wczytanych funkcjach
        for( unsigned j=0; j < repository.files_arr[i]->arr_size; ++j ) {
            if( strcmp( repository.files_arr[i]->fun_arr[j]->name, fun_name ) == 0 ) {
                return repository.files_arr[i]->fun_arr[j];
            }
        }
    }

    return NULL;
}

/* void remove_fun( analysed_file_t analysed_file, char* fun_name ) {
    int idx = -1;

    for( int i=0; i < analysed_file->arr_size; ++i ) {
        if( strcmp( analysed_file->fun_arr[i] -> name, fun_name ) == 0 ) {
            idx = i;
            break;
        }
    }

    if( idx < 0 ) {
        return;
    }

    fun_t to_delete = analysed_file->fun_arr[idx];
    analysed_file->fun_arr[idx] = analysed_file->fun_arr[ analysed_file->arr_size-1 ];
    --(analysed_file->arr_size);

    free_fun( to_delete );
} */


bool check_in_fun_arr( char* str ) {
    // iterowanie po wczytanych plikach
    for( unsigned i=0; i < repository.arr_size; ++i ) {
        // iterowanie po wczytanych funkcjach w pliku
        for( unsigned j=0; j < repository.files_arr[i]->arr_size; ++j ) {
            if( strcmp( repository.files_arr[i]->fun_arr[j]->name, str ) == 0 ) {
                return true;
            }
        }
    }

    return false;
}
