#ifndef _FUN_DICTIONARY_H_IS_INCLUDED_
#define _FUN_DICTIONARY_H_IS_INCLUDED_

// NAGŁÓWKI
#include <stdbool.h>

// STRUKTURA
typedef struct d {
    char* key;          // klucz elementu
    int value;          // wartość elementu
    struct d* next;     // wskaźnik na kolejny element słownika
} * fun_dictionary_t;


// PROTOTYPY POWIĄZANYCH FUNKCJI
int dict_get_value( fun_dictionary_t, char* );          // pobiera wartość słownika, która jest przypisana do podanego klucza
void dict_set_value( fun_dictionary_t, char*, int );    // ustawia na jakąś liczbę daną wartość słownika, która jest przypisana do podanego klucza
void dict_add( fun_dictionary_t*, char*, int );         // dodaje do słownika nowy element
// void dict_remove( fun_dictionary_t*, char* );        // usuwa z słownika element o podanym kluczu
void dict_free( fun_dictionary_t* );                    // usuwa wszystkie elementy ze słownika i zwalnia całą pamięć, która była przez niego zajmowana
void dict_print_all( fun_dictionary_t, unsigned );      // wypisuje w stdout wszystkie elementy, które znajdują się w podanym słowniku
bool dict_exist( fun_dictionary_t, char* );             // sprawdza czy w słowniku znajduje się element o podanym kluczu

#endif // _FUN_DICTIONARY_H_IS_INCLUDED_
