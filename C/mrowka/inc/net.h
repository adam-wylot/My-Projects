#ifndef NET_H_IS_INCLUDED
#define NET_H_IS_INCLUDED

#include <stdbool.h>

#include "net_gui.h"
#include "ant.h"


typedef struct {
    net_gui_t gui;

    unsigned rows;
    unsigned cols;
    bool** map; /// [x][y]
    ant_t ant;

}* net_t;

net_t net_init( unsigned rows, unsigned cols, direction_t ant_direction );
void net_free( net_t* );

void net_gen_by_perc( net_t, unsigned perc );
void net_make_moves( net_t, int amount );
void net_switch_tile( net_t, unsigned x, unsigned y );
void net_set_from_gui( net_t );

#endif /// NET_H_IS_INCLUDED
