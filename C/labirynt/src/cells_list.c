#include "cells_list.h"

#include <stdio.h>
#include <stdlib.h>

/* FUNCKJE LISTY */
void add_to_cells_list( cells_list_t* cells_list, cell_t cell ) {
    cells_list_t new = (cells_list_t)malloc( sizeof *new );
    if( new == NULL ) {
        fprintf( stderr, "[!] Błąd: nie udało się dodać komórki do ścieżki! (komórka: %p)\n", (void*)cell );
        return;
    }
    
    new -> cell = cell;
    new -> next = NULL;
    
    /* sprawdza czy były wcześniej jakieś elementy */
    if( *cells_list == NULL ) {
        *cells_list = new;
        return;
    }
    
    cells_list_t tmp_list = *cells_list;
    while( tmp_list -> next != NULL ) {
        tmp_list = tmp_list -> next;
    }
    
    tmp_list -> next = new;
}

cell_t get_from_cells_list( cells_list_t cells_list, int idx ) {
    cells_list_t tmp_ptr = cells_list;
    if( cells_list_size( tmp_ptr ) <= idx ) {
        return NULL;
    }

    for( int i=0; i < idx; ++i ) {
        tmp_ptr = tmp_ptr -> next;
    }
    
    return tmp_ptr -> cell;
}

int cells_list_size( cells_list_t cells_list ) {
    int n=0;
    while( cells_list != NULL ) {
        ++n;
        cells_list = cells_list -> next;
    }
    
    return n;
}

void clear_cells_list( cells_list_t cells_list ) {
    cells_list_t next_obj = NULL;
    
    while( cells_list != NULL ) {
        next_obj = cells_list -> next;
        free( cells_list );
        cells_list = next_obj;
    }
}

void print_cells_list( cells_list_t cells_list, int level ) {
    if( cells_list == NULL ) {
        fprintf( stderr, "[!] Błąd: lista jest pusta!\n" );
        return;
    }
    
    cell_t first_cell = NULL;
    cell_t second_cell = NULL;
    
    while( cells_list -> next != NULL ) {
        first_cell = cells_list -> cell;
        second_cell = cells_list -> next -> cell;
        
        printf( "%*s(%2d, %2d) --{%3.3lf}--> (%2d, %2d)\n",
               3*level,
               "",
               
               first_cell->x,
               first_cell->y,
               DIFF( first_cell->weight, second_cell->weight ),
               second_cell->x,
               second_cell->y );
        
        cells_list = cells_list -> next;
    }
}

bool is_in_cells_list( cells_list_t cells_list, cell_t cell ) {
    if( cells_list == NULL || cell == NULL ) { // zabezpieczenie
        return false;
    }
    
    while( cells_list != NULL ) {
        if( cells_list->cell == cell ) {
            return true;
        }
        
        cells_list = cells_list->next;
    }
    
    return false;
}
