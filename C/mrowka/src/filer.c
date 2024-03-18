#include "filer.h"
#include "repository.h"
#include "arguments.h"

#include <stdio.h>
#include <stdlib.h>

void file_make_from_gui( net_gui_t gui, unsigned iteration ) {

    // tworzenie nazwy
    char file_name[BUFSIZE];
    sprintf( file_name, "%s/%s_%d.txt", SAVE_PATH, arguments.file_prefix, iteration );

    // zapis do pliku
    file_save( gui, file_name );
}

int file_save( net_gui_t gui, char* file_path ) {
    FILE* file = fopen( file_path, "w" );
    if( file == NULL ){ // nie udało się otworzyć pliku
        char str[BUFSIZE];
        sprintf( str, "nie udało się otworzyć pliku \"%s\" do zapisu", file_path );

        push_error( str );
        return 1; /// porażka
    }

    // zapis do pliku
    for( unsigned i=0; i < (2*gui->rows + 1); ++i ) {
        for( unsigned j=0; j < (2*gui->cols + 1); ++j ) {
            fprintf( file, "%s", gui->char_map[j][i] );
        }
        fprintf( file, "\n" );
    }

    // zamknięcie pliki
    fclose( file );
    return 0; /// sukces
}

void file_load( net_gui_t gui, char* file_path ) {

    // otwieranie pliku
    FILE* file = fopen( file_path, "r" );
    if( file == NULL ){ // nie udało się otworzyć pliku
        char str[BUFSIZE];
        sprintf( str, "nie udało się otworzyć pliku \"%s\" do wczytania mapy", file_path );

        push_error( str );
        return;
    }

    // czytanie pliku
    int c=0;
    char read_str[CHARACTER_SIZE+1];

    for( int i=0; i < (2*gui->rows + 1); ++i ) {
        for( int j=0; j < (2*gui->cols + 1); ++j ) {

            // czytanie pliku
            for( int k=0; k < CHARACTER_SIZE; ++k ) {
                if( (c = getc( file )) != EOF ) {
                    read_str[k] = (char)c;

                    if( c == ' ' ) { // spacja
                        read_str[k+1] = '\0';
                        break;
                    }
                }
            }
            read_str[CHARACTER_SIZE] = '\0';
            strncpy( gui->char_map[j][i], read_str, CHARACTER_SIZE );
            // printf( "%s", read_str );
        }

        getc( file ); // pominięcie znaku końca linii
        // printf( "\n" );
    }
}
