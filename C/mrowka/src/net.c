#include "net.h"
#include "repository.h"
#include "arguments.h"
#include "filer.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>


net_t net_init( unsigned rows, unsigned cols, direction_t ant_direction ) {
// ALOKACJA PAMIĘCI
    net_t net = (net_t) malloc( sizeof *net );
    if( net == NULL ) {
        push_crit_error( "nie udało się zainicjować siatki - błąd pamięci" );
        return NULL;
    }

    // Mapa
    net->map = (bool**) malloc( cols * sizeof(bool*) );
    if( net->map == NULL ) { // zabezpieczenie
        free( net );

        push_crit_error( "nie udało się zainicjować mapy w siatce - błąd pamięci [1]" );
        return NULL;
    }
    for( unsigned i=0; i < cols; ++i ) {
        net->map[i] = (bool*) calloc( rows, sizeof(bool) ); // wypełnia mapę białymi polami
        if( net->map[i] == NULL ) { // zabezpieczenie
            // zwalnianie dotychczasowo zajętej pamięci
            for( unsigned j=0; j < i; ++j) {
                free( net->map[j] );
            }
            free( net->map );

            free( net );

            push_crit_error( "nie udało się zainicjować mapy w siatce - błąd pamięci [2]" );
            return NULL;
        }
    }

    // GUI
    net->gui = net_gui_init( rows, cols );
    if( net->gui == NULL ) { // wystąpił jakiś błąd przy inicjalizacji gui
        // zwalnianie dotychczasowo zajętej pamięci
        for( unsigned i=0; i < net->cols; ++i ) {
            free( net->map[i] );
        }
        free( net->map );

        free( net );

        push_crit_error( "nie udało się zainicjować odwzorowanie graficznego siatki - błąd pamięci" );
        return NULL;
    }

    // Mrówka
    net->ant = ant_init( rows, cols, -1, -1, ant_direction );
    if( net->ant == NULL ) { // błąd alokacji pamięci
        // zwalnianie dotychczasowo zajętej pamięci
        for( unsigned i=0; i < net->cols; ++i ) {
            free( net->map[i] );
        }
        free( net->map );

        net_gui_free( &(net->gui) );
        free( net );

        push_crit_error( "nie udało się zainicjować mrówki w siatce - błąd pamięci" );
        return NULL;
    }

// WARTOŚCI POCZĄTKOWE
    net->rows = rows;
    net->cols = cols;

    return net;
}

void net_free( net_t* net_ptr ) {
    if( *net_ptr == NULL ) { // zabezpieczenie
        push_error( "nie udało się zwolnić pamięci siatki - nie ma przydzielonej pamięci" );
        return;
    }

// Zwalnianie pamięci
    net_t net = *net_ptr;

    for( unsigned i=0; i < net->cols; ++i ) {
        free( net->map[i] );
    }
    free( net->map );

    net_gui_free( &(net->gui) );
    ant_free( &(net->ant) );
    free( net );

    *net_ptr = NULL;
}



void net_gen_by_perc( net_t net, unsigned perc ) {
    srand( time( NULL ) );

    unsigned cells_left = net->cols * net->rows;
    unsigned cells_to_fill = perc * cells_left / 100;

    for( int i=0; i < net->rows; ++i ) {
        for( int j=0; j < net->cols; ++j ) {

            if( rand() % cells_left < cells_to_fill ) {
                net->map[j][i] = 1;
                --cells_to_fill;
            }
            --cells_left;
        }
    }
}

void net_make_moves( net_t net, int amount ) {
    unsigned last_x = 0;
    unsigned last_y = 0;

    for( int i=0; i < amount; ++i ) {
        last_x = net->ant->x;
        last_y = net->ant->y;

        // wykonywanie ruchu
        ant_move( net->ant, net->map[ net->ant->x ][ net->ant->y ] );
        net_switch_tile( net, last_x, last_y );

        // ustawianie i rysowanie siatki
        if( arguments.print_all_flag ) { // rysowanie na bieżąco
            clear_console();
            if( net_gui_set( net->gui, net->map, net->rows, net->cols, net->ant ) == 0 ) { // wystąpił błąd
                net_gui_print( net->gui );
            }
            else {
                push_error( "nie udało się ustawić wartości w graficznym odzwierciedleniu siatki" );
            }
        }

        if( i >= (amount - MAX_FILES) ) { // ma zacząć tworzyć pliki
            if( net_gui_set( net->gui, net->map, net->rows, net->cols, net->ant ) == 0 ) { // wystąpił błąd
                file_make_from_gui( net->gui, i+1 );
            }
            else {
                push_error( "nie udało się ustawić wartości w graficznym odzwierciedleniu siatki" );
            }
        }
    }

    // rysowanie ostatniej iteracji
    if( !arguments.print_all_flag ) {
        if( net_gui_set( net->gui, net->map, net->rows, net->cols, net->ant ) == 0 ) { // wystąpił błąd
            net_gui_print( net->gui );
        }
        else {
            push_error( "nie udało się ustawić wartości w graficznym odzwierciedleniu siatki" );
        }
    }
}

void net_switch_tile( net_t net, unsigned x, unsigned y ) {
    // zabezpieczenie
    if( net->cols <= x || net->rows <= y ) {
        push_error( "nie udało się zmienić pola - podane pole wychodzi poza granice siatki" );
        return;
    }

    // zamiana pola
    if( net->map[x][y] == WHITE ) {
        net->map[x][y] = BLACK;
    }
    else { // czarne
        net->map[x][y] = WHITE;
    }
}

void net_set_from_gui( net_t net ) {
    if( net == NULL || net->gui == NULL ) { // zabezpieczenie
        push_error( "nie udało się ustawić siatki - siatka nie istnieje" );
        return;
    }

    // ustawianie mapy
    char* str;

    for( int i=0; i < net->rows; ++i ) {
        for( int j=0; j < net->cols; ++j ) {
            str = net_gui_get( net->gui, j, i );

            // sprawdza, czy to kwadrat i jeżeli tak to jaki
            if( strcmp(str, SQUARE_BLACK ) == 0 ) { // czarny kwadrat
                net->map[j][i] = 1;
            }
            else if( strcmp(str, SQUARE_WHITE ) == 0 ) { // biały kwadrat
                net->map[j][i] = 0;
            }

            // w takim razie jest to mrówka, a nie kwadrat
            else {
                net->ant->x = j;
                net->ant->y = i;

                // czarna:
                if( strcmp(str, ARROW_NORTH_BLACK ) == 0 ) {
                    net->map[j][i] = 1;
                    net->ant->direction = UP;
                }
                else if( strcmp(str, ARROW_EAST_BLACK ) == 0 ) {
                    net->map[j][i] = 1;
                    net->ant->direction = RIGHT;
                }
                else if( strcmp(str, ARROW_SOUTH_BLACK ) == 0 ) {
                    net->map[j][i] = 1;
                    net->ant->direction = DOWN;
                }
                else if( strcmp(str, ARROW_WEST_BLACK ) == 0 ) {
                    net->map[j][i] = 1;
                    net->ant->direction = LEFT;
                }

                    // biała:
                else if( strcmp(str, ARROW_NORTH_WHITE ) == 0 ) {
                    net->map[j][i] = 0;
                    net->ant->direction = UP;
                }
                else if( strcmp(str, ARROW_EAST_WHITE) == 0 ) {
                    net->map[j][i] = 0;
                    net->ant->direction = RIGHT;
                }
                else if( strcmp(str, ARROW_SOUTH_WHITE ) == 0 ) {
                    net->map[j][i] = 0;
                    net->ant->direction = DOWN;
                }
                else if( strcmp(str, ARROW_WEST_WHITE ) == 0 ) {
                    net->map[j][i] = 0;
                    net->ant->direction = LEFT;
                }
            }
        }
    }
}
