#include "repository.h"

#include <stdio.h>
#include <stdlib.h>

void init_rep( char* file_name ) {
    repository.file_name = file_name;

// rozpoznawanie systemu operacyjnego (do czyszczenia konsoli)
#ifdef _WIN32
    repository.oper_system = WIN32;
#else
    repository.oper_system = UNIX;
#endif
}

// wiadomości
void push_error( char* message ) {
    fprintf( stderr, "%s: błąd: %s!\n", repository.file_name, message );
}

void push_crit_error( char* message ) {
    push_error( message );
    send_message( "wystąpił błąd krytyczny - kończę działanie programu" );

    exit( EXIT_FAILURE ); /// porażka
}

void send_message( char* message ) {
    printf( "%s: %s.\n", repository.file_name, message );
}

void clear_console( void ) {
    switch( repository.oper_system ) {
        case WIN32: {
            system("cls");
            break;
        }
        case UNIX: {
            system("clear");
            break;
        }
    }
}
