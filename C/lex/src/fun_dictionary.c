// NAGŁÓWKI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "fun_dictionary.h"
#include "repository.h"

// DEFINICJE FUNKCJI
int dict_get_value( fun_dictionary_t dict, char* key ) {
    while( dict != NULL ) {
        // porównywanie kluczy
        if( strcmp( dict -> key, key ) == 0 ) {
            return dict -> value;
        }

        dict = dict -> next;
    }

    // minimalna wartość zmiennej int w wypadku nieznalezienia elementu o takim kluczu
    return INT_MIN;
}

void dict_set_value( fun_dictionary_t dict, char* key, int value ) {
    while( dict != NULL ) {
        // porównywanie kluczy
        if( strcmp( dict -> key, key ) == 0 ) {
            dict -> value = value;
            return;
        }

        dict = dict->next;
    }
}

void dict_add( fun_dictionary_t* dict_ptr, char* key , int value  ) {
    // Sprawdza, czy słownik nie jest przypadkiem pusty
    if( *dict_ptr == NULL ) {

        // alokowanie pamięci na pierwszy element słownika
        *dict_ptr = (fun_dictionary_t)malloc( sizeof **dict_ptr );
        if( *dict_ptr == NULL ) {
            char str[BUFSIZE];
            sprintf( str, "nie udało się dodać elementu \"%s\" do słownika - niepowodzenie podczas alokacji", key );

            push_error( str );
            return;
        }

        // alokowanie pamięci na klucz
        (*dict_ptr) -> key = (char*)malloc( ( strlen(key) + 1 ) * sizeof(char) );
        if( (*dict_ptr)->key == NULL ) {
            char str[BUFSIZE];
            sprintf( str, "nie udało się dodać elementu \"%s\" do słownika - niepowodzenie podczas alokacji klucza", key );

            free( *dict_ptr );
            push_error( str );
            return;
        }

// ustawianie wartości elementu
        strcpy( (*dict_ptr) -> key, key );
        (*dict_ptr) -> value = value;
        (*dict_ptr) -> next = NULL;

        return;
    }


// Natomiast, jeżeli nie był to pierwszy element słownika to:
    // szukanie ostatniego elementu w słowniku
    fun_dictionary_t tmp_dict = *dict_ptr;

    while( tmp_dict -> next != NULL ) {
        tmp_dict = tmp_dict -> next;
    }

    // alokowanie pamięci na element i na klucz
    fun_dictionary_t new = (fun_dictionary_t)malloc( sizeof *new );
    if( new == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać elementu \"%s\" do słownika - niepowodzenie podczas alokacji", key );

        push_error( str );
        return;
    }

    new -> key = (char*)malloc( ( strlen(key) + 1 ) * sizeof(char) );
    if( new->key == NULL ) {
        char str[BUFSIZE];
        sprintf( str, "nie udało się dodać elementu \"%s\" do słownika - niepowodzenie podczas alokacji klucza", key );

        free( new );
        push_error( str );
        return;
    }

    // ustawianie wartości elementu
    strcpy( new -> key, key );
    new -> value = value;
    new -> next = NULL;

    tmp_dict -> next = new;
}

/* void dict_remove( fun_dictionary_t* dict_ptr, char* key ) {
    if( dict_ptr == NULL || *dict_ptr == NULL ) { // zabezpieczenie
        char str[BUFSIZE];
        sprintf( str, "nie udało się usunąć elementu \"%s\" ze słownika - podany element nie istnieje", key );
        push_error( str );
        return;
    }

// kod dla pierwszego elementu w słowniku
    if( strcmp( (*dict_ptr) -> key, key  ) == 0 ) {
        fun_dictionary_t old = *dict_ptr;
        *dict_ptr = (*dict_ptr) -> next;

        free( old->key );
        free( old );
        return;
    }

// kod dla pozostałych elementów
    fun_dictionary_t prev = *dict_ptr;
    fun_dictionary_t to_delete = NULL;

    // szukanie elementu
    while( prev->next != NULL ) {
        // porównywanie kluczy
        if( strcmp( prev -> next -> key, key ) == 0 ) {
            to_delete = prev -> next;
            prev -> next = to_delete -> next;

            free( to_delete->key );
            free( to_delete );
            return;
        }

        prev = prev -> next;
    }
} */

void dict_free( fun_dictionary_t* dict_ptr ) {
    fun_dictionary_t old = NULL;

    // powtarzanie usuwania elementów, dopóki nie pozostanie nic
    while( *dict_ptr != NULL ) {
        old = *dict_ptr;
        *dict_ptr = (*dict_ptr)->next;

        free( old->key );
        free( old );
    }
}

void dict_print_all( fun_dictionary_t dict, unsigned level ) {
    while( dict != NULL ) {
        printf( "%*s%s", 3*level, "", dict->key );

        if( dict->value > 1 ) {
            printf( " (%d razy)", dict->value );
        }
        printf( "\n" );

        dict = dict->next;
    }
}

bool dict_exist( fun_dictionary_t dict, char* key ) {
    while( dict != NULL ) {
        if( strcmp( dict->key, key ) == 0 ) {
            return true;
        }
        dict = dict->next;
    }

    return false;
}
