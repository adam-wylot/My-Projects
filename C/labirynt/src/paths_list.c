#include "paths_list.h"
#include "cells_list.h"

#include <stdio.h>
#include <stdlib.h>

/* FUNCKJE LISTY */
path_t* add_to_paths_list( paths_list_t* paths_list ) {
    /* alokacja pamięci na nowy element */
    paths_list_t new = (paths_list_t)malloc( sizeof *new );
    if( new == NULL ) {
        printf( "[!] Błąd: nie udało się dodać ścieżki do listy!n" );
        return NULL;
    }
    
    /* wartości początkowe */
    new->path.cells_list = NULL;
    new->path.starting_cell = NULL;
    new->path.level = 0;
    new->path.cells_amount = 0;
    new->path.summed_weight = 0.;
    
    new->next = NULL;
    
    /* sprawdza czy były wcześniej jakieś elementy - jeżeli nie to ustawia nowy jako pierwszy element */
    if( *paths_list == NULL ) {
        *paths_list = new;
        return &(new->path);
    }
    
    /* szuka ostatniego elementu listy, aby wrzucić nowy na sam koniec */
    paths_list_t tmp_list = *paths_list;
    while( tmp_list -> next != NULL ) {
        tmp_list = tmp_list -> next;
    }
    
    tmp_list -> next = new;
    
    return &(new->path);
}

path_t* get_from_paths_list( paths_list_t paths_list, unsigned idx ) {
    /* jeżeli indeks wychodzi poza listę */
    if( paths_list_size( paths_list ) <= (int)idx ) {
        return NULL;
    }
    
    paths_list_t tmp_ptr = paths_list;

    /* szuka odpowiedniego elementu */
    for( unsigned i=0; i < idx; ++i ) {
        tmp_ptr = tmp_ptr -> next;
    }
    
    return &(tmp_ptr -> path);
}

int paths_list_size( paths_list_t paths_list ) {
    int n=0;
    while( paths_list != NULL ) {
        ++n;
        paths_list = paths_list -> next;
    }
    
    return n;
}

int get_path_index( paths_list_t paths_list, path_t path ) {
    int n=0;
    while( paths_list != NULL ) {
        if( compare_paths( paths_list -> path, path ) ) {
            return n;
        }
        
        paths_list = paths_list -> next;
        ++n;
    }
    
    return -1;
}

void print_paths_list( paths_list_t paths_list ) {
    path_t* actual_path = NULL;
    
    for( int i=1; paths_list != NULL; ++i ) {
        actual_path = &( paths_list->path );
        printf( "ŚCIEŻKA %d:\n", i );
        print_path( *actual_path, 1 );
        printf( "Łączna waga ścieżki: %.3lf\n\n", actual_path->summed_weight );
        
        paths_list = paths_list->next;
    }
}

void remove_from_paths_list( paths_list_t* paths_list_ptr, path_t path_to_delete ) {
    if( *paths_list_ptr == NULL || paths_list_ptr == NULL ) {
        fprintf( stderr, "[!] Błąd: podano element do usunięcia w pustej liście!\n" );
        return;
    }
    
    /* jeżeli do usunięcia jest pierwszy indeks */
    if( compare_paths( (*paths_list_ptr) -> path, path_to_delete ) ) {
        paths_list_t old = *paths_list_ptr;
        *paths_list_ptr = (*paths_list_ptr)->next;
        
        free( old );
        return;
    }
    
    /* każdy inny indeks */
    paths_list_t prev = *paths_list_ptr;
    paths_list_t tmp_paths_list = NULL;
    
    while( prev->next != NULL ) {
        if( compare_paths( prev->next->path, path_to_delete ) ) {
            tmp_paths_list = prev->next;
            prev->next = tmp_paths_list->next;
            
            free( tmp_paths_list );
            return;
        }
        
        prev = prev->next;
    }
    
    fprintf( stderr, "[!] Błąd: podano nieistniejący w liście element do usunięcia!\n" );
}

void clear_paths_list( paths_list_t* paths_list_ptr ) {
    paths_list_t next = NULL;
    
    while( *paths_list_ptr != NULL ) {
        clear_cells_list( (*paths_list_ptr)->path.cells_list );
        
        next = (*paths_list_ptr)->next;
        free( *paths_list_ptr );
        *paths_list_ptr = next;
    }
}

path_t* get_best_path( paths_list_t path_list ) {
    if( path_list == NULL ) {
        return NULL;
    }
    
    path_t* actual_best = &( path_list->path );
    path_list = path_list->next;
    
    while( path_list != NULL ) {
        // zmiana najlepszego
        if( path_list->path.summed_weight < actual_best->summed_weight ) {
            actual_best = &( path_list->path );
        }
        
        path_list = path_list->next;
    }
    
    return actual_best;
}
