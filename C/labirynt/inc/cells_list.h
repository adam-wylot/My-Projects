#ifndef _CELLS_LIST_H_IS_INCLUDED_
#define _CELLS_LIST_H_IS_INCLUDED_

#include <stdbool.h>

#include "cell.h"


/* MAKRA */
#define DIFF(a,b) a > b ? a - b : b - a

/* LISTY JEDNOKIERUNKOWE */
typedef struct c_l {
    cell_t cell;
    struct c_l* next;
} * cells_list_t;

/* FUNKCJE */
void add_to_cells_list( cells_list_t*, cell_t );
cell_t get_from_cells_list( cells_list_t, int );
int cells_list_size( cells_list_t );
void clear_cells_list( cells_list_t );
void print_cells_list( cells_list_t, int );
bool is_in_cells_list( cells_list_t, cell_t );

#endif
