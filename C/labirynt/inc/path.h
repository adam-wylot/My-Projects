#ifndef _PATH_H_IS_INCLUDED_
#define _PATH_H_IS_INCLUDED_

#include <stdbool.h>

#include "cell.h"
#include "cells_list.h"

/* ŚCIEŻKA */
typedef struct ph {
    cell_t starting_cell;
    cells_list_t cells_list;
    struct ph** children;
    int children_amount;
    int level;
    int cells_amount;
    double summed_weight;
}* path_t;

/* FUNKCJE */
void copy_path( path_t, path_t* );
void adjust_paths( path_t*, path_t* );
bool compare_paths( path_t, path_t );
void print_path( path_t, int );

#endif
