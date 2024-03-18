                    /* === NAGŁÓWKI === */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "repository.h"

                    /* === FUNKCJE === */
void init_rep( int argc, char** argv ) {
    repository.argc = argc;
    repository.argv = argv;
    
    /* rozpoznawanie systemu operacyjnego */
#ifdef _WIN32
    repository.oper_sys = WINDOWS;
#else
    repository.oper_sys = UNIX;
#endif
}

void set_variables( maze_t* maze_p ) {
/* ZMIENNE */
    int rows = repository.argc > 1 ? atoi( repository.argv[1] ) : 0; /* wymiar labiryntu */
    int cols = repository.argc > 2 ? atoi( repository.argv[2] ) : 0; /* wymiar labiryntu */
    
    /* ZIARNO LOSOWOŚCI */
    int seed = repository.argc > 3 ? atoi( repository.argv[3] ) : 0;
    if( seed == 0 ) { /* nie wpisano ziarna || nie udało się wczytać ziarna */
        seed = time(NULL);
    }
    
/* ZABEZPIECZENIA */
    if( rows == 0 ) { /* dla braku argumentu, lub źle wczytanego pyta o rozmiar*/
        clear_console();
        printf( "Podaj liczbę wierszy w labiryncie: " );
        scanf( "%d", &rows );
    }
    if( rows < 5 ) { /* Sprawdzanie spełnienia warunków */
        fprintf( stderr, "[!] %s: Minimalny rozmiar labiryntu to 5x5", repository.argv[0] );
        exit( EXIT_FAILURE );
    }
    
    if( cols == 0 ) { /* dla braku argumentu, lub źle wczytanego pyta o rozmiar*/
        clear_console();
        printf( "Podaj liczbę kolumn w labiryncie: " ); /* zalecane maksimum: 49 */
        scanf( "%d", &cols );
    }
    if( cols < 5 ) { /* Sprawdzanie spełnienia warunków */
        fprintf( stderr, "[!] %s: Minimalny rozmiar labiryntu to 5x5", repository.argv[0] );
        exit( EXIT_FAILURE );
    }
    
/* INICJALIZACJA LABIRYNTU + GENEROWANIE LABIRYNTU */
    *maze_p = init_maze( repository.argv[0], seed, rows, cols, -1, -1 );
}

void print_all( maze_t maze, graph_t graph ) {
/* ZMIENNE */
    int i=0;
    int n = maze->cols;
    
    /* centrowanie napisów oraz okreslanie ich minimalnej i maksymalnej długości */
    n = 2*n + 3;
    if( n < 15 ) {
        n = 15;
    }
    if( n > 101 ) {
        n = 101;
    }
    
/* -- RYSOWANIE -- */
    clear_console();
    
/* CZĘŚĆ WSTĘPNA */
    for( i=0; i < n; ++i ) { /* prosta linia */
        printf( "==" );
    }
    printf( "\n" );
    
    printf( "   Seed:       %d\n", maze->seed );
    printf( "   Rozmiar:    %dx%d\n", maze->cols, maze->rows );
    printf( "   Start:      (%2d, %2d)\n", maze->start_cell->x, maze->start_cell->y );
    printf( "   Koniec:     (%2d, %2d)\n", maze->end_cell->x, maze->end_cell->y );
    
    for( i=0; i < n; ++i ) { /* prosta linia */
        printf( "==" );
    }
    printf( "\n" );
    
/* LABIRYNT */
    for( i=0; i < n - 6; ++i ) { /* prosta linia */
        printf( "-" );
    }
    
    printf( " Labirynt:  " );
    
    for( i=0; i < n - 6; ++i ) { /* prosta linia */
        printf( "-" );
    }
    printf( "\n\n" );
    
    print_maze( maze );
    printf( "\n" );
    
    for( i=0; i < n; ++i ) { /* prosta linia */
        printf( "==" );
    }
    printf( "\n" );
    
/* GRAF */
    for( i=0; i < n - 4; ++i ) { /* prosta linia */
        printf( "-" );
    }
    printf( " Graf:  " );
    for( i=0; i < n - 4; ++i ) { /* prosta linia */
        printf( "-" );
    }
    printf( "\n\n" );
    
    print_graph( graph );

    for( i=0; i < n; ++i ) { /* prosta linia */
        printf( "==" );
    }
    printf( "\n" );
    
/* MACIERZ SĄSIEDZTWA */
    for( i=0; i < n - 11; ++i ) { /* prosta linia */
        printf( "-" );
    }
    printf( " Macierz sąsiedztwa:  " );
    for( i=0; i < n - 11; ++i ) { /* prosta linia */
        printf( "-" );
    }
    printf( "\n\n" );
    
    for( i=0; i < maze->cols * maze->rows; ++i ) {
        printf( "[%d]:  ", i+1 );
        for( int j=0; graph.adjacency_matrix[i][j] != 0; ++j ) {
            printf( "%d  ", graph.adjacency_matrix[i][j] );
        }
        printf( "\n" );
    }
    printf( "\n" );
    
    for( i=0; i < n; ++i ) { /* prosta linia */
        printf( "==" );
    }
    printf( "\n" );
    
/* WSZYSTKIE ŚCIEŻKI */
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    
    printf( " Dostępne ścieżki:  " );
    
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    printf( "\n\n" );
    
    // przejścia pomiędzy komórkami
    print_paths_list( graph.paths_list );

    for( i=0; i < n; ++i ) { // prosta linia
        printf( "==" );
    }
    printf( "\n" );
    
/* NAJLEPSZA ŚCIEŻKA */
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    
    printf( " Najlepsza ścieżka: " );
    
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    printf( "\n\n" );
    
    // ŚCIEŻKA
    printf( "ŚCIEŻKA %d:\n", get_path_index( graph.paths_list, *(graph.best_path_ptr) ) + 1 );
    print_path( *(graph.best_path_ptr), 1 );
    printf( "Łączna waga ścieżki: %.3lf\n\n", graph.best_path_ptr->summed_weight );

    for( i=0; i < n; ++i ) { // prosta linia
        printf( "==" );
    }
    
/* WSZYSTKIE KORYTARZE (pomocniczne przy debugowaniu) */
    /* printf( "\n" );
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    printf( " Dostępne korytarze:  " );
    for( i=0; i < n - 10; ++i ) { // prosta linia
        printf( "-" );
    }
    printf( "\n\n" );
    
    for( i=0; i < graph.cor_amount; ++i ) {
        if( graph.cor_arr[i].starting_cell == NULL ) {
            printf( "Starting cell: (null)\n" );
        }
        else {
            printf( "Starting cell: (%d,%d)\n", graph.cor_arr[i].starting_cell->x, graph.cor_arr[i].starting_cell->y );
        }
        print_cells_list( graph.cor_arr[i].cells_list, 0 );
        printf( "Summed weight: %lf\n", graph.cor_arr[i].summed_weight );
        printf( "\n" );
    }

    for( i=0; i < n; ++i ) { // prosta linia
        printf( "==" );
    } */
    printf( "\n\n" );
    
}

void clear_console( void ) {
    switch( repository.oper_sys ) {
        case WINDOWS: {
            system( "cls" );
            break;
        }
        case UNIX: {
            system( "clear");
            break;
        }
    }
}
