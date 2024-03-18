#include "ant.h"
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>

ant_t ant_init( unsigned rows, unsigned cols, int x, int y, unsigned direction ) {
    // błędne dane
    if( x < 0 ) {
        x = (int)cols / 2;
    }
    if( y < 0 ) {
        y = (int)rows / 2;
    }

    if( direction > 3 ) {
        direction = LEFT;
    }

// ALOKACJA PAMIĘCI
    ant_t ant = (ant_t) malloc( sizeof *ant );
    if( ant == NULL ) { // błąd alokacji
        return NULL;
    }

    ant->rows = rows;
    ant->cols = cols;
    ant->x = x;
    ant->y = y;
    ant->direction = direction;

    return ant;
}

void ant_free( ant_t* ant_ptr ) {
    if( *ant_ptr == NULL ) { // zabezpieczenie
        push_error( "nie udało się zwolnić pamięci mrówki - nie ma przydzielonej pamięci" );
        return;
    }

    free( *ant_ptr );
    *ant_ptr = NULL;
}

void ant_move( ant_t ant, bool tile ) {
    int tmp_direct = (int)ant->direction;

    if( tile == WHITE ) { // biały
        ++tmp_direct; // obrót w prawo
    }
    else if( tile == BLACK ) { // czarny
        --tmp_direct; // obrót w lewo
    }

// jeżeli obrót wyjdzie poza zakres, to poprawia wartość (UP to pierwsze element numeratora, natomiast LEFT to ostatni)
    if( tmp_direct < UP ) {
        tmp_direct = LEFT;
    }
    else if( tmp_direct > LEFT ) {
        tmp_direct = UP;
    }

// przejście o pole przed siebie
    ant->direction = (direction_t) tmp_direct;

    switch( ant->direction ) {
        case UP: {
            ant->y -= 1;
            break;
        }
        case RIGHT: {
            ant->x += 1;
            break;
        }
        case DOWN: {
            ant->y += 1;
            break;
        }
        case LEFT: {
            ant->x -= 1;
            break;
        }
    }

// poprawa pozycji, jeżeli wyszło poza siatkę
    if( ant->x < 0 ) {
        ant->x = (int)ant->cols - 1;
    }
    else if( ant->x >= ant->cols ) {
        ant->x = 0;
    }
    else if( ant->y < 0 ) {
        ant->y = (int)ant->rows - 1;
    }
    else if( ant->y >= ant->rows ) {
        ant->y = 0;
    }
}
