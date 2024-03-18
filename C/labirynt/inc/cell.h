#ifndef _CELL_H_
#define _CELL_H_

                    /* === NAGŁÓWKI === */
#include <stdbool.h>

                    /* === ZMIENNE === */
/* STAŁE */
#define WALL_STR "██" /* Wall string - znaki, które będą rysowane w miejscu ściany */
#define EMPTYF_STR "  " /* Empty Field string - znaki, które będą rysowane w miejscu pustego pola */

/* KIERUNKI */
enum destinations { TOP=0, RIGHT, BOTTOM, LEFT }; /* przechowuje wartości liczbowe dla poszczególnych kierunków */

            /* === STRUKTURA === */
typedef struct
{
    unsigned index;
    bool* is_open; /* 1 (true) - jest ścieżka; 0 (false) - nie ma ścieżki */
    /*  Kierunki:
     *  TOP=0       - góra
     *  RIGHT=1     - prawo
     *  BOTTOM=2    - dół
     *  LEFT=3      - lewo
     */
    bool was_here;
    unsigned x;
    unsigned y;
    double weight; /* waga */
    
} * cell_t;

                    /* === FUNKCJE === */
cell_t init_cell( unsigned, unsigned, unsigned );   /* inicjuje komórkę                         */
void free_cell( cell_t );       /* zwalnia pamięć zajmowaną przez komórkę   */

#endif
