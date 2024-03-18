                    /* === NAGŁÓWKI === */
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"

                    /* === FUNKCJE === */
cell_t init_cell( unsigned index, unsigned x, unsigned y ) {
    cell_t cell = (cell_t)malloc( sizeof *cell );
    if( cell == NULL ) { /* zabezpieczenie */
        return NULL;
    }
    
/* ZMIENNE */
    cell -> index = index;
    cell -> was_here = 0;
    cell -> x = x;
    cell -> y = y;
    cell -> weight = (double)rand() / RAND_MAX * 10 ;
    
/* ALOKACJA PAMIĘCI */
    cell -> is_open = (bool*)calloc( 4, sizeof(bool) );
    if( cell -> is_open == NULL ) { /* zabezpieczenie */
        free( cell );
        return NULL;
    }
    
    return cell;
}

void free_cell( cell_t cell ) {
    free( cell -> is_open );
    free( cell );
}
