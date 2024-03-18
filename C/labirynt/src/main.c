                    /* === NAGŁÓWKI === */
#include <stdio.h>

#include "repository.h"
#include "maze.h"
#include "graph.h"

                    /* ---=== MAIN ===--- */
int main(int argc, char **argv)
{
/* ZMIENNE */
    maze_t maze = NULL;
    graph_t graph;
    
/* PROGRAM */
    init_rep( argc, argv );
    set_variables( &maze );
    graph = init_graph( maze );
    
    print_all( maze, graph );

/* KOŃCZENIE PROGRAMU */
    free_graph( graph );
    free_maze( maze );
    
    return 0; /* sukces */
}
