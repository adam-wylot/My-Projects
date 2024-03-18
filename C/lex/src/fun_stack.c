/* NAGŁÓWKI */
#include <stdio.h>
#include <stdlib.h>

#include "fun_stack.h"

/* DEFINICJE FUNKCJI */
// jest to prosta funkcja, nawet niepotrzebna, ale dodanie jej zwiększa czytelność kodu
int fun_stack_line_value( fun_stack_t fun_stack ) {
    if( fun_stack_is_empty( fun_stack ) ) { /* zabezpieczenie */
        return -1;
    }
    return fun_stack->line;
}

void fun_stack_push( fun_stack_t* fun_stack_ptr, char* fun_name, unsigned line ) {
    if( fun_name == NULL ) { /* zabezpieczenie */
        return;
    }
    
/* alokacja nowego elementu */
    fun_stack_t new = (fun_stack_t)malloc( sizeof *new );
    if( new == NULL ) { /* zabezpieczenie */
        return;
    }
    
/* przypisanie wartości do nowego elementu stosu */
    new->line = (int)line;
    new->fun_name = fun_name;
    new->previous = *fun_stack_ptr;
    *fun_stack_ptr = new;
}

char* fun_stack_get( fun_stack_t* fun_stack_ptr ) {
    if( fun_stack_is_empty( *fun_stack_ptr ) ) { /* zabezpieczenie */
        return NULL;
    }
    
/* rozróżnienie elementów stosu */
    fun_stack_t old_top = *fun_stack_ptr;
    fun_stack_t new_top = old_top->previous;
    
/* przypisanie wartości */
    char* temp = old_top->fun_name;
    *fun_stack_ptr = new_top;

// usunięcie starego elementu i zwrócenie wartości, którą przechowywał
    free( old_top );
    return temp;
}

// kolejna prosta funkcja, która zwiększa wyłącznie czytelność kodu
char* fun_stack_seek( fun_stack_t fun_stack ) {
    if( fun_stack_is_empty( fun_stack ) ) { /* zabezpieczenie */
        return NULL;
    }

    return fun_stack->fun_name;
}

bool fun_stack_is_empty( fun_stack_t fun_stack ) {
    if( fun_stack == NULL ) { /* zabezpieczenie */
        return true;
    }
    return false;
}

void fun_stack_clear( fun_stack_t* fun_stack_ptr ) {
    fun_stack_t old_top = NULL;

    // usuwanie dopóki nie dojdzie do końca stosu
    while( !fun_stack_is_empty( *fun_stack_ptr ) ) {
        old_top = *fun_stack_ptr;
        *fun_stack_ptr = old_top->previous;

        free( old_top );
    }
}
