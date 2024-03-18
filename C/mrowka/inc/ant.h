#ifndef ANT_H_IS_INCLUDED
#define ANT_H_IS_INCLUDED

#include "stdbool.h"

typedef enum { UP=0, RIGHT, DOWN, LEFT } direction_t;
/** w górę:     0
  * w prawo:    1
  * w dół:      2
  * w lewo:     3
  */

typedef struct {
    int x;
    int y;
    unsigned rows;
    unsigned cols;
    direction_t direction;

}* ant_t;

ant_t ant_init( unsigned rows, unsigned cols, int x, int y, unsigned direction );
void ant_free( ant_t* );

void ant_move( ant_t, bool tile ); //0 - biały ORAZ 1 - czarny

#endif /// ANT_H_IS_INCLUDED
