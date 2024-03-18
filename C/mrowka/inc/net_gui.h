#ifndef NET_GUI_H_IS_INCLUDED
#define NET_GUI_H_IS_INCLUDED

#include "printer.h"
#include "ant.h"

typedef struct {
    unsigned rows;
    unsigned cols;
    char*** char_map; ///[x][y][ char ]

}* net_gui_t;

net_gui_t net_gui_init( unsigned rows, unsigned cols );
void net_gui_free( net_gui_t* );
char* net_gui_get( net_gui_t, unsigned x, unsigned y );
void net_gui_print( net_gui_t );

void net_gui_make_borders( net_gui_t );
int net_gui_set( net_gui_t, bool** map, unsigned rows, unsigned cols, ant_t ant );
void net_gui_set_ant( net_gui_t, bool** map, ant_t );
void net_gui_set_tiles( net_gui_t, bool** map );

#endif /// NET_GUI_H_IS_INCLUDED
