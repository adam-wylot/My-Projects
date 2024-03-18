#ifndef ARGUMENTS_H_IS_INCLUDED
#define ARGUMENTS_H_IS_INCLUDED

#include <stdbool.h>

#include "ant.h" // direction_t

/** Ten obiekt określa jak mają być czytane parametry wywołania programu oraz zapisuje je
  * w postaci czytelnej dla programu.    */

struct {
    // parametry obowiązkowe
    unsigned rows;
    unsigned cols;
    unsigned iterations;
    char* file_prefix;
    direction_t direction;

    // parametry opcjonalne
    bool print_all_flag;

    char* map_path; // sciezka do mapy, jesli jest ona wgrywana do programu
    bool map_flag; // czy flaga do wgrania mapy, zostal wprowadzony jako parametr < -m (sciezka) >

    int percentage; // procent czarnych pol do generacji
    bool perc_flag; // czy uzytkownik chce zeby wygenerowac mape < -p (0...10) >

} arguments;

void init_arguments( int argc, char** argv );
void read_required_args( char** argv );
void read_additional_args( int argc, char** argv );
void check_args( void );

int arg_read_file( char* path );

#endif /// ARGUMENTS_H_IS_INCLUDED
