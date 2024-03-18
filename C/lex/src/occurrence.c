// NAGŁÓWKI
#include "occurrence.h"

#include <stdio.h>

// DEFINICJE FUNKCJI
void occ_print( occurrence_t occurrence ) {
    printf( "Plik: \"%s\" | Start: %d | Koniec: %d\n", occurrence->file_name, occurrence->start, occurrence->end );
}
