#include "path.h"

#include <stdio.h>

void copy_path( path_t dest, path_t source ) {
    dest -> cells_amount = source->cells_amount;
    dest -> summed_weight = source->summed_weight;
    
    /* kopiowanie listy komórek */
    for( int i=0; i < source->cells_amount; ++i ) {
        add_to_cells_list( &(dest->cells_list), get_from_cells_list( source->cells_list, i ) );
    }
}

void adjust_paths( path_t main_path, path_t second_path ) {
    main_path->cells_amount += second_path->cells_amount;
    main_path->summed_weight += second_path->summed_weight;
    
    /* kopiowanie listy komórek */
    for( int i=0; i < second_path->cells_amount; ++i ) {
        add_to_cells_list( &(main_path->cells_list), get_from_cells_list( second_path->cells_list, i ) );
    }
}

bool compare_paths( path_t first, path_t second ) {
    if( first.starting_cell != second.starting_cell )   return 0;
    if( first.cells_list != second.cells_list )         return 0;
    
    if( first.level != second.level )                   return 0;
    if( first.cells_amount != second.cells_amount )     return 0;
    if( first.summed_weight != second.summed_weight )   return 0;
       
    return 1;
}

void print_path( path_t path, int level ) {
    cell_t cell = path.starting_cell;
    if( cell == NULL ) {
        cell = get_from_cells_list( path.cells_list, 0 );
    }
        
    // Początek
    printf( "%*sWEJŚCIE  -----------> (%2d, %2d)\n",
           3*level,
           "",
           
           cell->x,
           cell->y );
    
    // Środek
    print_cells_list( path.cells_list, level );
    
    // Koniec
    cell = get_from_cells_list( path.cells_list, path.cells_amount-1 );
    printf( "%*s(%2d, %2d) ----------->  WYJŚCIE\n",
           3*level,
           "",
           
           cell->x,
           cell->y );
}
