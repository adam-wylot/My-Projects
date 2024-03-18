#ifndef _REP_H_
#define _REP_H_
                    /* === NAGŁÓWKI === */
#include "maze.h"
#include "graph.h"

/* MAKRA */
#define DIFF(a,b) a > b ? a - b : b - a

                    /* === ZMIENNE === */
typedef enum { WINDOWS=0, UNIX } system_t;

                    /* === STRUKTURA MAGAZYNU === */
struct {
    int argc;
    char** argv;
    system_t oper_sys;
    
} repository;

                    /* === FUNKCJE === */
void init_rep( int, char** );       /* inicjalizuje magazyn         */
void set_variables( maze_t* );      /* ustawia wartości zmiennych   */
void print_all( maze_t, graph_t );  /* rysuje wszystko              */
void clear_console( void );         /* czyści zawartość konsoli     */

#endif
