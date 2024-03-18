#ifndef _OCCURRENCE_H_IS_INCLUDED_
#define _OCCURRENCE_H_IS_INCLUDED_

// NUMERATOR
typedef enum {
    prototype=0, definition, usage
} occType_t;

// STRUKTURA
typedef struct {
    occType_t type;     // jaki typ wystąpienia to jest

    char* file_name;    // w jakim pliku opisywane wystąpienie się znajduje
    unsigned start;     // startowa linijka wystąpienia
    unsigned end;       // końcowa linijka wystąpienia
} * occurrence_t;

// PROTOTYPY POWIĄZANYCH FUNKCJI
void occ_print( occurrence_t );     // rysuje w stdout wszystkie informacje o podanym w argumencie wystąpieniu

#endif // _OCCURRENCE_H_IS_INCLUDED_
