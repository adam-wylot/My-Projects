#ifndef _ANALYSER_H_IS_INCLUDED_
#define _ANALYSER_H_IS_INCLUDED_

// NAGŁÓWKI
#include <stdio.h>
#include <stdbool.h>

#include "analysed_files.h"

// PROTOTYPY POWIĄZANYCH FUNKCJI
bool analyse_file( analysed_file_t );           // funkcja analizująca leksję danego pliku

    // operacje na pliku
int get_next_char( analysed_file_t, char* );    // pobiera następny "niepusty" znak z pliku (odpowiednio liczy przejścia do kolejnych linijek) oraz tworzy z następujących po sobie znaków słowo
void skip_to_space( analysed_file_t, char* );   // pomija wszystkie znaki, aż do kolejnej napotkanej spacji
void skip_line( analysed_file_t );              // pomija wszystkie znaki, aż do końca bierzącej linii

#endif // _ANALYSER_H_IS_INCLUDED_
