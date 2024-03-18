#include "net_gui.h"
#include "repository.h"
#include "printer.h"

#include <stdlib.h>

net_gui_t net_gui_init( unsigned rows, unsigned cols ) {

// ALOKACJA PAMIĘCI
    net_gui_t gui = (net_gui_t) malloc( sizeof *gui );
    if( gui == NULL ) { // zabezpieczenie
        return NULL;
    }

    // kolumny
    gui->char_map = (char***) malloc( (2 * cols + 1) * sizeof(char**) );
    if( gui->char_map == NULL ) { // zabezpieczenie
        free( gui );
        return NULL;
    }

    // wiersze
    for( unsigned i=0; i < (2 * cols + 1); ++i ) {
        gui->char_map[i] = (char**) malloc( (2 * rows + 1) * sizeof(char*) );
        if( gui->char_map[i] == NULL ) { // zabezpieczenie

            // zwalnianie dotychczasowo zajętej pamięci
            for( unsigned k=0; k < i; ++k ) {
                free( gui->char_map[k] );
            }
            free( gui->char_map );

            free( gui );
            return NULL;
        }

        // pojedyncze komórki tablicy
        for( unsigned j=0; j < (2 * rows + 1); ++j ) {
            gui->char_map[i][j] = (char*) malloc( (CHARACTER_SIZE + 1) * sizeof(char) ); // +1 dla znaku '\0'
            if( gui->char_map[i][j] == NULL ) { // zabezpieczenie

                // zwalnianie dotychczasowo zajętej pamięci
                for( unsigned k=0; k < i; ++k ) {
                    for( unsigned l=0; l < (2 * rows + 1); ++l ) {
                       free( gui->char_map[k][l] );
                    }
                    free( gui->char_map[k] );
                }

                for( unsigned l=0; l < j; ++l ) {
                    free( gui->char_map[i][l] );
                }
                free( gui->char_map[i] );

                free( gui->char_map );
                free( gui );
                return NULL;
            }
        }
    }

// WARTOŚCI POCZĄTKOWE
    gui->rows = rows;
    gui->cols = cols;

// GRANICE SIATKI
    net_gui_make_borders( gui );

    return gui;
}

void net_gui_free( net_gui_t* gui_ptr ) {
    if( *gui_ptr == NULL ) { // zabezpieczenie
        push_error( "nie udało się zwolnić pamięci graficznej siatki - nie ma przydzielonej pamięci" );
        return;
    }

// Zwalnianie pamięci
    net_gui_t gui = *gui_ptr;

    // czyszczenie tablicy
    for( unsigned i=0; i < (2*gui->cols + 1); ++i ) {
        /*for( unsigned j=0; j < (2*gui->rows + 1); ++j ) {
            free( gui->char_map[i][j] );
        }*/

        free( gui->char_map[i] );
    }
    free( gui->char_map );

    // czyszczenie obiektu
    free( gui );
    *gui_ptr = NULL;
}

void net_gui_print( net_gui_t gui ) {
    for( unsigned i=0; i < (2 * gui->rows + 1); ++i ) {
        for( unsigned j=0; j < (2 * gui->cols + 1); ++j ) {
            printf( "%s", gui->char_map[j][i] );
        }
        printf( "\n" );
    }
}

char* net_gui_get( net_gui_t gui, unsigned x, unsigned y ) {
    if( gui->cols <= x || gui->rows <= y ) {
        return NULL;
    }

    return gui->char_map[ 2*x + 1 ][ 2*y + 1 ];
}




void net_gui_make_borders( net_gui_t gui ) {
// LINIE POZIOME
    for( unsigned i=0; i < gui->rows; ++i ) {
        for( unsigned j=0; j < (2 * gui->cols + 1); ++j ) {
            strncpy( gui->char_map[ j ][ 2*i ], LINE_HORIZONTAL, CHARACTER_SIZE );
        }
    }
    for( unsigned j=0; j < (2 * gui->cols + 1); ++j ) {
        strncpy(gui->char_map[ j ][ 2 * gui->rows ], LINE_HORIZONTAL, CHARACTER_SIZE );
    }

// LINIE PIONOWE
    for( unsigned i=0; i < gui->cols; ++i ) {
        for( unsigned j=0; j < (2 * gui->rows + 1); ++j ) {
            strncpy( gui->char_map[ 2*i ][ j ], LINE_VERTICAL, CHARACTER_SIZE );
        }
    }
    for( unsigned j=0; j < (2 * gui->rows + 1); ++j ) {
        strncpy( gui->char_map[ 2 * gui->cols ][ j ], LINE_VERTICAL, CHARACTER_SIZE );
    }

// ROGI
    strncpy( gui->char_map[ 0 ][ 0 ], LINE_DOWN_RIGHT, CHARACTER_SIZE );                     /// lewy-górny
    strncpy( gui->char_map[ 2*gui->cols ][ 0 ], LINE_DOWN_LEFT, CHARACTER_SIZE );            /// prawy-górny
    strncpy( gui->char_map[ 0 ][ 2*gui->rows ], LINE_UP_RIGHT, CHARACTER_SIZE );             /// lewy-dolny
    strncpy( gui->char_map[ 2*gui->cols ][ 2*gui->rows ], LINE_UP_LEFT, CHARACTER_SIZE );    /// prawy-dolny

// ŚRODEK
    for( unsigned i=0; i < gui->rows - 1; ++i ) {
        for( unsigned j=0; j < gui->cols - 1; ++j ) {
            strncpy( gui->char_map[ 2 + 2*j ][ 2 + 2*i ], LINE_CROSS_MIDDLE, CHARACTER_SIZE);
        }
    }

// ŚCIANY
    // górna
    for( unsigned i=0; i < gui->cols - 1; ++i ) {
        strncpy( gui->char_map[ 2 + 2*i ][ 0 ], LINE_CROSS_TOP, CHARACTER_SIZE );
    }

    // dolna
    for( unsigned i=0; i < gui->cols - 1; ++i ) {
        strncpy( gui->char_map[ 2 + 2*i ][ 2*gui->rows ], LINE_CROSS_BOTTOM, CHARACTER_SIZE );
    }

    // lewa
    for( unsigned i=0; i < gui->rows - 1; ++i ) {
        strncpy( gui->char_map[ 0 ][ 2 + 2*i ], LINE_CROSS_LEFT, CHARACTER_SIZE );
    }

    // prawa
    for( unsigned i=0; i < gui->rows - 1; ++i ) {
        strncpy( gui->char_map[ 2*gui->cols ][ 2 + 2*i ], LINE_CROSS_RIGHT, CHARACTER_SIZE );
    }
}




int net_gui_set( net_gui_t gui, bool** map, unsigned rows, unsigned cols, ant_t ant ) {
    if( rows != gui->rows || cols != gui->cols ) { // zabezpieczenie przez niepasującą tablicą
        return 1; /// porażka
    }

    net_gui_set_tiles( gui, map );
    net_gui_set_ant( gui, map, ant );

    return 0; /// sukces
}

void net_gui_set_ant( net_gui_t gui, bool** map, ant_t ant ) {
    unsigned x = 2*ant->x + 1;
    unsigned y = 2*ant->y + 1;
    
    switch( ant->direction ) {
        case UP: {
            if( map[ ant->x ][ ant->y ] == WHITE ) {
                strncpy( gui->char_map[x][y], ARROW_NORTH_WHITE, CHARACTER_SIZE );
            }
            else { // czarny
                strncpy( gui->char_map[x][y], ARROW_NORTH_BLACK, CHARACTER_SIZE );
            }
            break;
        }
        case RIGHT: {
            if( map[ ant->x ][ ant->y ] == WHITE ) {
                strncpy( gui->char_map[x][y], ARROW_EAST_WHITE, CHARACTER_SIZE );
            }
            else { // czarny
                strncpy( gui->char_map[x][y], ARROW_EAST_BLACK, CHARACTER_SIZE );
            }
            break;
        }
        case DOWN: {
            if( map[ ant->x ][ ant->y ] == WHITE ) {
                strncpy( gui->char_map[x][y], ARROW_SOUTH_WHITE, CHARACTER_SIZE );
            }
            else { // czarny
                strncpy( gui->char_map[x][y], ARROW_SOUTH_BLACK, CHARACTER_SIZE );
            }
            break;
        }
        case LEFT: {
            if( map[ ant->x ][ ant->y ] == WHITE ) {
                strncpy( gui->char_map[x][y], ARROW_WEST_WHITE, CHARACTER_SIZE );
            }
            else { // czarny
                strncpy( gui->char_map[x][y], ARROW_WEST_BLACK, CHARACTER_SIZE );
            }
            break;
        }
    }
}

void net_gui_set_tiles( net_gui_t gui, bool** map ) {
    for( unsigned i = 0; i < gui->rows; ++i ) {
        for( unsigned j = 0; j < gui->cols; ++j ) {
            if( map[j][i] == 1 ) { // czarny
                strncpy( net_gui_get( gui, j, i ), SQUARE_BLACK, CHARACTER_SIZE );
            } else { // biały
                strncpy( net_gui_get( gui, j, i ), SQUARE_WHITE, CHARACTER_SIZE );
            }
        }
    }
}
