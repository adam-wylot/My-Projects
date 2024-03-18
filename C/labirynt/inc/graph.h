#ifndef _GRAPH_H_IS_INCLUDED_
#define _GRAPH_H_IS_INCLUDED_

#include <stdbool.h>

#include "maze.h"
#include "path.h"
#include "paths_list.h"
#include "cells_list.h"
#include "cellstack.h"

/* STRUKTURA */
typedef struct {
    maze_t maze;
    int** adjacency_matrix; /* macierz sąsiedztwa */
    paths_list_t paths_list; /* lista jednokierunkowa ścieżek */
    path_t* best_path_ptr;
    
    path_t* cor_arr; // corridors
    int cor_capacity;
    int cor_amount;

} graph_t;

/* PROTOTYPY FUNKCJI */
graph_t init_graph( maze_t );
void print_graph( graph_t );
void free_graph( graph_t );

void make_corridors( graph_t* );
void make_paths( graph_t*, path_t*, path_t );
path_t* add_corridor( graph_t*, cell_t );

#endif
