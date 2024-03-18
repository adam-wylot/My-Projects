                    /* === NAGŁÓWKI === */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "cell.h"
#include "cellstack.h"

                    /* === FUNKCJE === */
maze_t init_maze( char* file_name_s, int seed, int rows, int cols, int start_x, int end_x ) {
    srand( seed );
    if( start_x < 0 ) {
        start_x = rand() % cols;
    }
    if( end_x < 0 ) {
        end_x = rand() % cols;
    }
    
/* Alokacja pamięci */
    maze_t maze = alloc_mem( rows, cols );
    if( maze == NULL ) {
        fprintf( stderr, "[!] %s: Nie udało się zainicjalizować labiryntu!\n", file_name_s );
        exit( EXIT_FAILURE );
    }
    
/* Ustawianie zmiennych */
    maze -> seed = seed;
    maze -> rows = rows;
    maze -> cols = cols;
    maze -> start_cell = maze -> map_2arr[start_x][0];
    maze -> end_cell = maze -> map_2arr[end_x][ rows-1 ];
    
/* generowanie ścieżek w labiryncie */
    gen_path( maze );
    gen_rest_maze( maze );
    
    return maze;
}

maze_t alloc_mem( int rows, int cols ) {
    int i=0, j=0;
    
/* Alokacja pamięci na obiekt struktury labiryntu */
    maze_t maze = (maze_t)malloc( sizeof *maze );
    if( maze == NULL ) { /* zabezpieczenie */
        return NULL;
    }
    
/* Alokacja pamięci na mapę labiryntu */
    maze -> map_2arr = (cell_t**)malloc( cols * sizeof *(maze -> map_2arr) );
    if( maze -> map_2arr == NULL ) { /* zabezpieczenie */
        free( maze );
        return NULL;
    }
    
/* Alokacja pamięci na wskaźniki na komórki */
    for( i=0; i < cols; ++i ) {
        maze -> map_2arr[i] = (cell_t*)malloc( rows * sizeof **(maze -> map_2arr) );
        if( maze -> map_2arr[i] == NULL ) { /* zabezpieczenie */
            int k=0;
            for( k=0; k < i; ++k ) {
                free( maze -> map_2arr[k] );
            }
            
            free( maze -> map_2arr );
            free( maze );
            
            return NULL;
        }
    }
    
/* Alokacja pamięci na pojedyńcze komórki */
    for( i=0; i < rows; ++i ) {
        for( j=0; j < cols; ++j )
        {
            maze -> map_2arr[j][i] = init_cell( (int)( i*cols + j ) + 1, j, i ); /* index, x, y */
            if( maze -> map_2arr[j][i] == NULL ) { /* zabezpieczenie */
                int k=0, l=0;
                
                for( k=0; k < j; ++k ) { /* [!] czyszczenie niedokończonego w inizjalizacji wiersza */
                    free_cell( maze -> map_2arr[k][i] );
                }
                
                for( k=0; k < i; ++k ) { /* [!] czyszczenie pozostałych wierszy w całości */
                    for( l=0; l < cols; ++l ) {
                        free_cell( maze -> map_2arr[l][k] );
                    }
                }
                
                for( k=0; k < cols; ++k ) { /* [!] czyszczenie pamięci wskaźników na komórki */
                    free( maze -> map_2arr[k] );
                }
                
                free( maze -> map_2arr );
                free( maze );
                
                return NULL;
            }
        }
    }
    
    return maze;
}

void gen_path( maze_t maze ) {
/* ZMIENNE */
    cstack_t cells_stack = NULL; /* stos do przechowywania komórek */
    cell_t current_cell = NULL; /* aktualnie sprawdzana komórka */
    
    int unv[4]; /* unvisited - tablica, która przechowa komórki sąsiadujące z aktualnie sprawdzaną, w których nie było jeszcze nigdy generatora */
    int unv_count=0; /* do zliczania ile jest tych nieodwiedzonych "sąsiadów" */
    int x=0, y=0; /* będą używane w celu zwiększenia czytelności kodu */
    
    /* oznacza komórkę startową za odwiedzoną i wrzuca ją na stos */
    maze -> start_cell -> was_here = 1;
    push( &cells_stack, maze -> start_cell );
    
/* ALGORYTM GENEROWANIA ŚCIEŻEK */
    while( !is_empty( cells_stack ) ) {
        current_cell = pop( &cells_stack );
        x = current_cell -> x;
        y = current_cell -> y;
        
        /* sprawdza czy generator nie doszedł do końca; jeżeli tak to idzie do kolejnej iteracji; w ten sposób stworzy dodatkową ścieżkę */
        if( current_cell == maze->end_cell ) {
            break;
        }
        
        /* sprawdzanie, które z sąsiednich komórek były odwiedzone: pierwszy warunek ogranicza algorytm, żeby nie próbował wyjśc poza labirynt (prowadziłoby to do Segmentation Fault); drugi warunek sprawdza każdego z "sąsiadów", czy algorytm już przez nich przechodził */
        unv_count = 0;
        
        if( y-1 >= 0 && maze -> map_2arr[x][y-1] -> was_here == 0  ) { /* TOP */
            unv[ unv_count++ ] = TOP;
        }
        
        if( x+1 < maze -> cols && maze -> map_2arr[x+1][y] -> was_here == 0) { /* RIGHT */
            unv[ unv_count++ ] = RIGHT;
        }
        
        if( y+1 < maze -> rows && maze -> map_2arr[x][y+1] -> was_here == 0 ) { /* BOTTOM */
            unv[ unv_count++ ] = BOTTOM;
        }
        
        if( x-1 >= 0 && maze -> map_2arr[x-1][y] -> was_here == 0 ) { /* LEFT */
            unv[ unv_count++ ] = LEFT;
        }
        
        /* jeżeli nie ma nieodwiedzonych "sąsiadów" - przechodzi do kolejnej iteracji */
        if( unv_count == 0 ) {
            continue;
        }
        
        /* w innym wypadku wrzuca aktualnie badaną komórke na stos, żeby później do niej wrócić i przechodzi do wybrania dalszego kierunku ścieżki */
        push( &cells_stack, current_cell );
        
        /* losowanie jednego z "sąsiadów", wykonywanie połączenia oraz wrzucanie go na stos */
        switch( unv[ rand() % unv_count ] ) {
            case TOP: {
                current_cell -> is_open[TOP] = 1;
                maze -> map_2arr[x][y-1] -> was_here = 1;
                push( &cells_stack, maze -> map_2arr[x][y-1] );
                break;
            }
            case RIGHT: {
                current_cell -> is_open[RIGHT] = 1;
                maze -> map_2arr[x+1][y] -> was_here = 1;
                push( &cells_stack, maze -> map_2arr[x+1][y] );
                break;
            }
            case BOTTOM: {
                current_cell -> is_open[BOTTOM] = 1;
                maze -> map_2arr[x][y+1] -> was_here = 1;
                push( &cells_stack, maze -> map_2arr[x][y+1] );
                break;
            }
            case LEFT: {
                current_cell -> is_open[LEFT] = 1;
                maze -> map_2arr[x-1][y] -> was_here = 1;
                push( &cells_stack, maze -> map_2arr[x-1][y] );
                break;
            }
        }
    }
}

void gen_rest_maze( maze_t maze ) {
/* ZMIENNE */
    cstack_t path_stack = NULL;
    cstack_t cells_stack = NULL;
    cell_t current_cell = NULL;
    cell_t next_cell = NULL;
    int rows = maze->rows;
    int cols = maze->cols;
    int x=0, y=0;
    
    /* pierwszym krokiem jest przejście przez wygenerowaną ścieżkę, wrzucenie jej komórek na stos i zaznaczenie losowych punktów na nieodwiedzone, aby utworzyć nowe połączenia */
    current_cell = maze -> end_cell;
    x = current_cell -> x;
    y = current_cell -> y;
    
    push( &cells_stack, current_cell );
    push( &path_stack, current_cell );
    
    while( !is_empty( cells_stack ) ) {
        current_cell = pop( &cells_stack );
        x = current_cell -> x;
        y = current_cell -> y;
        
        if( current_cell == maze -> start_cell ) {
            break;
        }
        
        if( y+1 < rows && maze -> map_2arr[x][y+1] -> is_open[TOP] ) { /* BOTTOM */
            next_cell = maze -> map_2arr[x][y+1];
        }
        else if( x > 0 && maze -> map_2arr[x-1][y] -> is_open[RIGHT] ) { /* LEFT */
            next_cell = maze -> map_2arr[x-1][y];
        }
        else if( x+1 < cols && maze -> map_2arr[x+1][y] -> is_open[LEFT] ) { /* RIGHT */
            next_cell = maze -> map_2arr[x+1][y];
        }
        else if( y > 0 && maze -> map_2arr[x][y-1] -> is_open[BOTTOM] ) { /* TOP */
            next_cell = maze -> map_2arr[x][y-1];
        }
        else { /* jak nie znajdzie drogi to przechodzi do kolejnej iteracji pętli */
            continue;
        }
        
        push( &cells_stack, current_cell );
        push( &cells_stack, next_cell );
        push( &path_stack, next_cell );
        
        if( rand() % 100 < 5 ) {
            next_cell -> was_here = 0;
        }
    }
    
    clear_stack( &cells_stack );
    maze -> end_cell -> was_here = 0; /* ustawia, że nigdy nie było generatora w końcowej komórce w celu stworzenia nowej ścieżki */
    
    /* w tym momencie w path_stack mamy przechowaną całą wygenerowaną ścieżkę */
    /* następnie generator znowu przechodzi przez ścieżkę i jeżeli wokół niej znajdują sie nieodwiedzone punkty to przechodzi przez nie i tworzy nowe ścieżki */
    
    while( !is_empty( path_stack ) ) {
        current_cell = pop( &path_stack );
        x = current_cell -> x;
        y = current_cell -> y;
        current_cell -> was_here = 1;
        
        if( y > 0 && maze -> map_2arr[x][y-1] -> was_here == 0 ) { /* TOP */
            next_cell = maze -> map_2arr[x][y-1];
            current_cell -> is_open[TOP] = 1;
        }
        else if( x > 0 && maze -> map_2arr[x-1][y] -> was_here == 0 ) { /* LEFT */
            next_cell = maze -> map_2arr[x-1][y];
            current_cell -> is_open[LEFT] = 1;
        }
        else if( x+1 < cols && maze -> map_2arr[x+1][y] -> was_here == 0 ) { /* RIGHT */
            next_cell = maze -> map_2arr[x+1][y];
            current_cell -> is_open[RIGHT] = 1;
        }
        else if( y+1 < rows && maze -> map_2arr[x][y+1] -> was_here == 0 ) { /* BOTTOM */
            next_cell = maze -> map_2arr[x][y+1];
            current_cell -> is_open[BOTTOM] = 1;
        }
        else { /* jak nie znajdzie drogi to przechodzi do kolejnej iteracji pętli */
            continue;
        }
        
        push( &path_stack, current_cell );
        push( &path_stack, next_cell );
        
/* dla znalezionego przejścia wykonuje cały generator ścieżek: */
        int unv[4]; /* unvisited - tablica, która przechowa komórki sąsiadujące z aktualnie sprawdzaną, w których nie było jeszcze nigdy generatora */
        int unv_count=0; /* do zliczania ile jest tych nieodwiedzonych "sąsiadów" */
        
        /* oznacza komórkę startową za odwiedzoną i wrzuca ją na stos */
        next_cell -> was_here = 1;
        push( &cells_stack, next_cell );
        
    /* ALGORYTM GENEROWANIA ŚCIEŻEK */
        while( !is_empty( cells_stack ) ) {
            current_cell = pop( &cells_stack );
            x = current_cell -> x;
            y = current_cell -> y;
            
            /* sprawdzanie, które z sąsiednich komórek były odwiedzone: pierwszy warunek ogranicza algorytm, żeby nie próbował wyjśc poza labirynt (prowadziłoby to do Segmentation Fault); drugi warunek sprawdza każdego z "sąsiadów", czy algorytm już przez nich przechodził */
            unv_count = 0;
            
            if( y-1 >= 0 && maze -> map_2arr[x][y-1] -> was_here == 0  ) { /* TOP */
                unv[ unv_count++ ] = TOP;
            }
            
            if( x+1 < maze -> cols && maze -> map_2arr[x+1][y] -> was_here == 0) { /* RIGHT */
                unv[ unv_count++ ] = RIGHT;
            }
            
            if( y+1 < maze -> rows && maze -> map_2arr[x][y+1] -> was_here == 0 ) { /* BOTTOM */
                unv[ unv_count++ ] = BOTTOM;
            }
            
            if( x-1 >= 0 && maze -> map_2arr[x-1][y] -> was_here == 0 ) { /* LEFT */
                unv[ unv_count++ ] = LEFT;
            }
            
            /* jeżeli nie ma nieodwiedzonych "sąsiadów" - przechodzi do kolejnej iteracji */
            if( unv_count == 0 ) {
                continue;
            }
            
            /* w innym wypadku wrzuca aktualnie badaną komórke na stos, żeby później do niej wrócić i przechodzi do wybrania dalszego kierunku ścieżki */
            push( &cells_stack, current_cell );
            
            /* losowanie jednego z "sąsiadów", wykonywanie połączenia oraz wrzucanie go na stos */
            switch( unv[ rand() % unv_count ] ) {
                case TOP: {
                    current_cell -> is_open[TOP] = 1;
                    maze -> map_2arr[x][y-1] -> was_here = 1;
                    push( &cells_stack, maze -> map_2arr[x][y-1] );
                    break;
                }
                case RIGHT: {
                    current_cell -> is_open[RIGHT] = 1;
                    maze -> map_2arr[x+1][y] -> was_here = 1;
                    push( &cells_stack, maze -> map_2arr[x+1][y] );
                    break;
                }
                case BOTTOM: {
                    current_cell -> is_open[BOTTOM] = 1;
                    maze -> map_2arr[x][y+1] -> was_here = 1;
                    push( &cells_stack, maze -> map_2arr[x][y+1] );
                    break;
                }
                case LEFT: {
                    current_cell -> is_open[LEFT] = 1;
                    maze -> map_2arr[x-1][y] -> was_here = 1;
                    push( &cells_stack, maze -> map_2arr[x-1][y] );
                    break;
                }
            }
        }
    }
}

void print_maze( maze_t maze ) {
/* ZMIENNE */
    cell_t cell = NULL;
    int rows = maze -> rows;
    int cols = maze -> cols;
    int x_tmp = maze -> start_cell -> x; /* zmienna, która przechowa pozycję [x] wejścia */
    int i=0, j=0;
    
                                    /* === RYSOWANIE === */
/* NUMEROWANIE KOLUMN */
    int num=0;
    if( cols <= 100 ) { /* w innym wypadku wizualizacja jest nierówna (przez większą liczbę cyfr w numeracji niż znaków ma pole) */
        int tmp = rows-1;
        while( tmp > 0 ) {
            ++num;
            tmp /= 10;
        }
        
        printf( "%*s ", num, "" );
        for( i=0; i < cols; ++i ) {
            printf( "%s%2d", EMPTYF_STR, i );
        }
        printf( "\n%*s ", num, "" );
    }

    
        /* -- RYSOWANIE ŚCIAN -- */
/* GÓRNA ŚCIANA */
    for( i=0; i < cols; ++i ) {
        cell = maze -> map_2arr[i][0];
        
        if( i == x_tmp ) { /* szukanie wejścia i robienie tam przerwy */
            printf( "%s%s", WALL_STR, EMPTYF_STR );
        }
        else {
            printf( "%s%s", WALL_STR, WALL_STR );
        }
    }
    printf( "%s\n", WALL_STR );
    
    
/* ŚRODKOWE WIERSZE - ŚCIANY WEWNĘTRZNE (oprócz ostatniego wiersza) */
    for( i=0; i < rows-1; ++i ) { /* iterowanie po wierszach - zmiana y */
        if( cols <= 100 ) { /* numeruje wiersze, jeżeli jest taka możliwość */
            printf( "%*d ", num, i );
        }
        
        /* środek - prawa ściana LUB środek - lewa ściana komórki po prawej */
        printf( "%s", WALL_STR );
        for( j=0; j < cols; ++j ) {
            cell = maze -> map_2arr[j][i];
            
            printf( "%s", EMPTYF_STR ); /* puste pole w środku komórki */
            /* sprawdzanie prawej ściany: */
            if( cell -> is_open[RIGHT] || ( j+1 < cols && maze -> map_2arr[j+1][i] -> is_open[LEFT] ) ) {
                printf( "%s", EMPTYF_STR );
            }
            else {
                printf( "%s", WALL_STR );
            }
        }
        printf( "\n" );
        
        /* dół - dolna ściana LUB góra - górna ściana niższej komórki */
        if( cols <= 100 ) { /* do numerowania wierszy */
            printf( "%*s ", num, "" );
        }
        printf( "%s", WALL_STR);
        
        for( j=0; j < cols; ++j ) {
            cell = maze -> map_2arr[j][i];
            
            if( cell -> is_open[BOTTOM] || ( i+1 < rows && maze -> map_2arr[j][i+1] -> is_open[TOP] ) ) {
                printf( "%s", EMPTYF_STR );
            }
            else {
                printf( "%s", WALL_STR );
            }
            
            printf( "%s", WALL_STR );
        }
        printf( "\n" );
    }
    
    
/* OSTATNI WIERSZ */
    if( cols <= 100 ) {
        printf( "%*d ", num, rows-1 );
    } /* numerowanie ostatniego wiersza, jeżeli jest taka możliwość */
    printf( "%s", WALL_STR );
    
    for( i=0; i < cols; ++i ) { /* środek - prawa ściana LUB środek - lewa ściana komórki po prawej */
        cell = maze -> map_2arr[i][rows-1];
        
        printf( "%s", EMPTYF_STR );
        if( cell -> is_open[RIGHT] || ( i+1 < cols && maze -> map_2arr[i+1][rows-1] -> is_open[LEFT] ) ) {
            printf( "%s", EMPTYF_STR );
        }
        else {
            printf( "%s", WALL_STR );
        }
    }
    printf( "\n" );
    
/* DOLNA ŚCIANA */
    if( cols <= 100 ) { /* przerwa do numerowania ostatniego wiersza | dolnej ściany */
        printf( "%*s ", num, "" );
    }
    x_tmp = maze -> end_cell -> x; /* zmienna, która przechowa przez chwilę pozycję [x] wyjścia */
    
    for( i=0; i < cols; ++i ) {
        cell = maze -> map_2arr[i][rows-1];
        
        if( i == x_tmp ) { /* szukanie wyjścia i robienie tam przerwy */
            printf( "%s%s", WALL_STR, EMPTYF_STR );
        }
        else {
            printf( "%s%s", WALL_STR, WALL_STR );
        }
    }
    printf( "%s\n", WALL_STR );
}

void free_maze( maze_t maze ) {
/* ZMIENNE */
    int rows = maze -> rows;
    int cols = maze -> cols;
    int i=0, j=0;
    
/* ZWALNIANIE PAMIĘCI */
    for( i=0; i < cols; ++i ) {
        for( j=0; j < rows; ++j ) {
            free_cell( maze -> map_2arr[i][j] );
        }
        free( maze -> map_2arr[i] );
    }
    
    free( maze -> map_2arr );
    free( maze );
}
