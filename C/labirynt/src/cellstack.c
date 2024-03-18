                    /* === NAGŁÓWKI === */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "cellstack.h"

                    /* === FUNKCJE === */
cell_t pop( cstack_t* stack_p ) {
    if( is_empty( *stack_p ) ) { /* zabezpieczenie */
        return NULL;
    }
    
    cstack_t old_top = *stack_p;
    cell_t pop_cell = old_top -> cell;
    
    *stack_p = old_top -> prev_p;
    
    /* printf( "[%d, %d] CZYSZCZE ELEMENT STOSU (p -> %p)\n", pop_cell -> x, pop_cell -> y, old_top -> prev_p ); */
    free( old_top );
    return pop_cell;
}

cell_t top( cstack_t* stack_ptr ) {
    if( is_empty( *stack_ptr ) ) { /* zabezpieczenie */
        return NULL;
    }
    
    return (*stack_ptr) -> cell;
}

void push( cstack_t* stack_p, cell_t cell_ ) {
    cstack_t new_top = (cstack_t)malloc( sizeof *new_top );
    if( new_top == NULL ) {
        return;
    }
    
    new_top -> cell = cell_;
    new_top -> prev_p = *stack_p;
    
    *stack_p = new_top;
}

bool is_empty( cstack_t stack ) {
    if( stack == NULL ){
        return true;
    }
    return false;
}

void clear_stack( cstack_t* stack_ptr ) {
    while( !is_empty( *stack_ptr ) ) {
        pop( stack_ptr );
    }
}
