#ifndef _MAZE_H_
#define _MAZE_H_

                    /* === NAGŁÓWKI === */
#include "cell.h"

                    /* === STRUKTURA LABIRYNTU === */
typedef struct
{
    int seed;
    int rows;
    int cols;
    cell_t start_cell;
    cell_t end_cell;
    cell_t **map_2arr; /* [x][y] */
    
}* maze_t;

                    /* === FUNKCJE === */
maze_t init_maze( char*, int, int, int, int, int ); /* inicjalizuje labirynt (nazwa pliku; seed; l. wierszy; l. kolumn; wejście (x); wyjście (x) ) */
maze_t alloc_mem( int, int );       /* służy do alokacji pamięci na "obraz" labiryntu ( wiersze; kolumny )  */
void gen_path( maze_t );            /* generuje ścieżkę w labiryncie                                        */
void gen_rest_maze( maze_t );       /* generuje resztę labiryntu                                            */
void print_maze( maze_t );          /* rysuje labirynt                                                      */
void free_maze( maze_t );           /* zwalnia pamięć zajmowaną przez labirynt                              */

#endif
