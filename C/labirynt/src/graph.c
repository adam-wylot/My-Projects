#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

graph_t init_graph( maze_t maze ) {
    graph_t graph;
    graph.maze = maze;
    
    graph.adjacency_matrix = (int**)malloc( maze->rows * maze->cols * sizeof(int*) );
    if( graph.adjacency_matrix == NULL ) {
        printf( "[!] Błąd: nie udało się stworzyć grafu!\n" );
        exit( EXIT_FAILURE );
    }
    for( int i=0; i < maze->rows * maze->cols; ++i ) {
        graph.adjacency_matrix[i] = (int*)calloc( 3, sizeof(int) );
        if( graph.adjacency_matrix[i] == NULL ) {
            for( int k=0; k < i; ++k ) {
                free( graph.adjacency_matrix[k] );
            }
            free( graph.adjacency_matrix );
            
            printf( "[!] Błąd: nie udało się stworzyć grafu!\n" );
            exit( EXIT_FAILURE );
        }
    }
    
    
    graph.cor_arr = (path_t*)malloc( 2 * sizeof( path_t ) );
    if( graph.cor_arr == NULL ) {
        printf( "[!] Błąd: nie udało się stworzyć grafu!\n" );
        exit( EXIT_FAILURE );
    }
    graph.cor_capacity = 2;
    graph.cor_amount = 1;
    
    graph.paths_list = NULL;
    graph.best_path_ptr = NULL;
    
    path_t* open_path_ptr = &(graph.cor_arr[0]);
    open_path_ptr -> starting_cell = NULL;
    open_path_ptr -> cells_list = NULL;
    open_path_ptr -> level = 0;
    open_path_ptr -> cells_amount = 0;
    open_path_ptr -> summed_weight = 0.;

    make_corridors( &graph );
    
    make_paths( &graph, add_to_paths_list( &(graph.paths_list) ), graph.cor_arr[0] );
    
    graph.best_path_ptr = get_best_path( graph.paths_list );
    
    return graph;
}

void make_corridors( graph_t* graph_ptr ) {
    /* ZMIENNE */
    maze_t maze = graph_ptr -> maze;
    int rows = maze -> rows;
    int cols = maze -> cols;
    
    cstack_t cells_stack = NULL;
    cstack_t crossways_stack = NULL; // służy do liczenia zagłębienia ścieżki
    
    cell_t current_cell = NULL;
    cell_t connected_cell = NULL;
    
    bool do_new_path = 0; /* przechowuje informację o tym czy był przeskok w rysowaniu pomiędzy odnogami grafu */
    int path_idx = 0;
    int level = 0;
    int x=0, y=0;
    
    cell_t unv_arr[3];
    int unv_dir_arr[3];
    int unv_counter=0;
    int cross_counter = 0;

    /* alokowanie tablicy 3dim */
    bool*** checked_3arr = (bool***)malloc( maze->cols * sizeof *checked_3arr );
    if( checked_3arr == NULL ) { /* zabezpieczenie */
        printf( "[!] Błąd: nie udało się wygenerować grafu!\n" );
        return;
    }
    
    for( int i=0; i < maze->cols; ++i ) {
        checked_3arr[i] = (bool**)malloc( maze->rows * sizeof **checked_3arr );
        
        if( checked_3arr[i] == NULL ) { /* zabezpieczenie */
            /* zwalnianie całej zaalokowanej wcześniej pamięci */
            for( int k=0; k < i; ++k ) {
                free( checked_3arr[k] );
            }
            free( checked_3arr );
            
            /* kończenie programu */
            printf( "[!] Błąd: nie udało się wygenerować grafu!\n" );
            return;
        }
        
        for( int j=0; j < maze->rows; ++j ) {
            checked_3arr[i][j] = (bool*)calloc( 4, sizeof(bool) );
            
            if( checked_3arr[i][j] == NULL ) { /* zabezpieczenie */
                /* zwalnianie całej zaalokowanej wcześniej pamięci */
                for( int k=0; k < j; ++k ) {
                    free( checked_3arr[i][k] );
                }
                free( checked_3arr[i] );
                
                for( int k=0; k < i; ++k ) {
                    for( int l=0; l < maze->rows; ++l ) {
                        free( checked_3arr[k][l] );
                    }
                    free( checked_3arr[k] );
                }
                
                free( checked_3arr );
                     
                /* kończenie programu */
                printf( "[!] Błąd: nie udało się wygenerować grafu!\n" );
                return;
            }
        }
    }
    

    
/* ALGORYTM */
    /* wrzucenie pierwszej komórki algorytmu na stos oraz dodanie jej do pierwszej ściezki (ścieżki wejściowej) */
    current_cell = maze -> start_cell;
    add_to_cells_list( &graph_ptr -> cor_arr[ path_idx ].cells_list, current_cell );
    ++(graph_ptr -> cor_arr[path_idx].cells_amount);
    
    push( &cells_stack, current_cell );
        
    /* algorytm wykonuje się dopóki coś jest na stosie, czyli istnieją niesprawdzone komórki (jeżel chodzi o ich połączenia)  */
    while( !is_empty( cells_stack ) ) {
        /* pobranie komórki z góry stosu i badanie jej */
        current_cell = pop( &cells_stack );
        x = current_cell -> x;
        y = current_cell -> y;
        
/* SPRAWDZANIE CZY ZNAJDUJĘ SIĘ NA SKRZYŻOWANIU DRÓG */
        /* Zmienne */
        cross_counter = 0;
        
        /* liczenie ilości dróg wchodzących do komórki */
        if( y-1 >= 0 && maze -> map_2arr[x][y-1] -> is_open[BOTTOM] ) { /* TOP */
            ++cross_counter;
        }
        if( x-1 >= 0 && maze -> map_2arr[x-1][y] -> is_open[RIGHT] ) { /* LEFT */
            ++cross_counter;
        }
        if( y+1 < rows && maze -> map_2arr[x][y+1] -> is_open[TOP] ) { /* BOTTOM */
            ++cross_counter;
        }
        if( x+1 < cols && maze -> map_2arr[x+1][y] -> is_open[LEFT] ) { /* RIGHT */
            ++cross_counter;
        }
        
        if( cross_counter > 1 ) {
            do_new_path = 1;
        }
        
/* TWORZENIE ROZWIDLEŃ I KONTYUNUACJI DRÓG */
        /* Zmienne */
        unv_counter = 0;
        
        /* liczenie ilości możliwych przejść */
        if( current_cell -> is_open[TOP] && !checked_3arr[x][y][TOP] ) { /* TOP */
            unv_dir_arr[ unv_counter ] = TOP;
            unv_arr[ unv_counter++ ] = graph_ptr -> maze -> map_2arr[x][y-1];
        }
        if( current_cell -> is_open[LEFT] && !checked_3arr[x][y][LEFT] ) { /* LEFT */
            unv_dir_arr[ unv_counter ] = LEFT;
            unv_arr[ unv_counter++ ] = graph_ptr -> maze -> map_2arr[x-1][y];
        }
        if( current_cell -> is_open[BOTTOM] && !checked_3arr[x][y][BOTTOM] ) { /* BOTTOM */
            unv_dir_arr[ unv_counter ] = BOTTOM;
            unv_arr[ unv_counter++ ] = graph_ptr -> maze -> map_2arr[x][y+1];
        }
        if( current_cell -> is_open[RIGHT] && !checked_3arr[x][y][RIGHT] ) { /* RIGHT */
            unv_dir_arr[ unv_counter ] = RIGHT;
            unv_arr[ unv_counter++ ] = graph_ptr -> maze -> map_2arr[x+1][y];
        }
        
/* DODATKOWE WARUNKI ALGORYTMU */
        /* sprawdza czy algorytm DFS przeszedł przez wcześniejsze skrzyżowanie - jeżeli tak to obniża poziom zagłębienia ścieżki */
        if( top( &crossways_stack ) == current_cell ) {
            pop( &crossways_stack );
            --level;
        }
        
        /* sprawdzenie czy dotarliśmy do wyjścia */
        if( current_cell == graph_ptr -> maze -> end_cell ) {
            do_new_path = 1; /* zmuszenie algorytmu do zrobienia nowej ścieżki */
        }
        
        /* sprawdzanie czy nie znalazło żadnego nowego połączenia w danej komórce - koniec aktualnej ścieżki, DFS zaczyna wracać po komórkach, w których już był */
        if( unv_counter == 0 ) {
            do_new_path = 1;
            continue;
        }
        
/* DODATKOWE WARUNKI ALGORYTMU - KONTYNUACJA TWORZENIA ŚCIEŻEK */
        /* sprawdza czy znajduje się na rozwidleniu - jeżeli tak to tworzy nową ścieżkę */
        if( unv_counter > 1 ) {
            do_new_path = 1;
        }
            
        /* sprawdzanie czy powinno zrobić nową ścieżkę */
        if( do_new_path ) {
            ++level;
            push( &crossways_stack, current_cell );
            
            add_corridor( graph_ptr, current_cell );
            ++path_idx;

            graph_ptr -> cor_arr[ path_idx ].level = level;

            do_new_path = 0;
        }
        
/* KONTYNUOWANIE TWORZENIA POJEDYŃCZEJ ŚCIEŻKI */
        /* wybranie pierwszej z dostępnych dróg na rozwidleniu (zawsze bierze pierwszą znalezioną) */
        connected_cell = unv_arr[0];
        checked_3arr[x][y][ unv_dir_arr[0] ] = 1;
        
        /* dodanie wagi do korytarza */
        graph_ptr -> cor_arr[path_idx].summed_weight += DIFF( current_cell->weight, connected_cell->weight );
        
        /* dodanie indeksu połączonej komórki do macierzy sąsiedztwa */
        int tmp=0;
        while( graph_ptr -> adjacency_matrix[ current_cell->index-1 ][tmp] != 0 ) {
            ++tmp;
        }
        
        graph_ptr -> adjacency_matrix[ current_cell->index-1 ][tmp] = connected_cell->index;
        
        /* wrzucanie na stos komórek, które razem tworzą przejście */
        push( &cells_stack, current_cell );
        push( &cells_stack, connected_cell );
        
        /* dodanie komórki, do której przechodzi do aktualnie rozpatrywanej drogi */
        add_to_cells_list( &(graph_ptr -> cor_arr[path_idx].cells_list), connected_cell );
        ++(graph_ptr -> cor_arr[path_idx].cells_amount);
    }
    
/* KOŃCZENIE ALGORYTMU */
    /* zwalnianie pamięci */
    for( int i=0; i < maze->cols; ++i ) {
        for( int j=0; j < maze->rows; ++j ) {
            free( checked_3arr[i][j] );
        }
        free( checked_3arr[i] );
    }
    free( checked_3arr );
                     
    clear_stack( &cells_stack );
    clear_stack( &crossways_stack );
}

void make_paths( graph_t* graph_ptr, path_t mother_path_ptr, path_t corridor ) {
    /* dodanie korytarza do ścieżki */
    adjust_paths( mother_path_ptr, &corridor );
    
    cell_t last_cell = get_from_cells_list( mother_path_ptr->cells_list, mother_path_ptr->cells_amount-1 );
    
    /* dotarło do końca? - warunek kończący zagnieżdżenie rekurencyjne */
    if( last_cell == graph_ptr->maze->end_cell ) {
        return;
    }
    
/* ZMIENNE */
    path_t connected_cor_arr[3];
    int counter = 0;
    
    /* szukanie ilości możliwych kontynuacji i zliczanie ich */
    for( int i=0; i < graph_ptr -> cor_amount; ++i ) {
        if( graph_ptr->cor_arr[i].starting_cell == last_cell ) {
            if( !is_in_cells_list( mother_path_ptr->cells_list, get_from_cells_list(graph_ptr->cor_arr[i].cells_list, 0 ) ) ) {
                connected_cor_arr[ counter++ ] = graph_ptr->cor_arr[i];
            }
        }
    }
    
    /* brak kontynuacji - usuwa ścieżkę i kończy zagłębienie rekurencyjne */
    if( counter == 0 ) {
        remove_from_paths_list( &(graph_ptr->paths_list), *mother_path_ptr );
        return;
    }
    
/* NOWE ŚCIEŻKI (o ile jakieś są) */
    for( int i=1; i < counter; ++i ) {
        path_t* new_path_ptr = add_to_paths_list( &(graph_ptr->paths_list) );
        copy_path( *mother_path_ptr, new_path_ptr );
        make_paths( graph_ptr, new_path_ptr, connected_cor_arr[i] );
    }
    
    /* kontynuacja aktualnej ścieżki */
    make_paths( graph_ptr, mother_path_ptr, connected_cor_arr[0] );
}

path_t add_corridor( graph_t* graph_ptr, cell_t starting_cell ) {
    // powiększenie pojemności (gdy jest taka potrzeba)
    if( graph_ptr -> cor_capacity <= graph_ptr -> cor_amount ) {
        path_t* old_ptr = graph_ptr -> cor_arr;
        
        graph_ptr -> cor_arr = (path_t*)realloc( graph_ptr -> cor_arr, 2 * (graph_ptr -> cor_capacity) * sizeof( path_t ) );
        if( graph_ptr -> cor_arr == NULL ) { /* zabezpieczenie */
            printf( "[!] Błąd: nie udało się dodać ścieżki do grafu!\n" );
            graph_ptr -> cor_arr = old_ptr;
            exit( EXIT_FAILURE );
        }
        
        graph_ptr -> cor_capacity *= 2;
    }
    
// ALOKACJA PAMIĘCI
    // alokowanie pamięci na element path_t
    path_t new = (path_t) malloc( sizeof *new );
    if( new == NULL ) {
        printf( "[!] Błąd: nie udało się utworzyć grafu!\n" );
        exit( EXIT_FAILURE );
    }
    
    
    // alokowanie pamięci na tablicę
    new.children = (path_t*) malloc( 3 * sizeof(path_t) );
    if( graph_ptr -> cor_arr[ graph_ptr->cor_amount ].children == NULL ) {
        printf( "[!] Błąd: nie udało się utworzyć grafu!\n" );
        
        free( new );
        exit( EXIT_FAILURE );
    }
    
    /* wartości początkowe */
    new.cells_list = NULL;
    new.cells_amount = 0;
    new.children_amount = 0;
    new.starting_cell = starting_cell;
    new.level = 0;
    new.summed_weight = 0.;
    
    graph_ptr -> cor_arr[ graph_ptr->cor_amount ] = new;
    
    return new;
}

/* RYSOWANIE */
void print_graph( graph_t graph ) {
    cell_t first_cell = NULL;
    cell_t second_cell = NULL;
    int i=0;
    int level=0;
    
    /* rysowanie 0-wego zagłębienia */
    second_cell = get_from_cells_list( graph.cor_arr[0].cells_list, 0 );
    
    printf( "WEJŚCIE  -----------> (%2d, %2d)\n",
           second_cell->x,
           second_cell->y );
    
    print_cells_list( graph.cor_arr[0].cells_list, 0 );
    
    for( i=1; i < graph.cor_amount; ++i ) {
        first_cell = graph.cor_arr[i].starting_cell;
        second_cell = get_from_cells_list( graph.cor_arr[i].cells_list, 0 );
        level = graph.cor_arr[i].level;
        
        printf( "%*s(%2d, %2d) --{%3.3lf}--> (%2d, %2d)\n",
               3*level,
               "",
               
               first_cell->x,
               first_cell->y,
               DIFF( first_cell->weight, second_cell->weight ),
               second_cell->x,
               second_cell->y );
        
        print_cells_list( graph.cor_arr[i].cells_list, level );
        
        /* sprawdzenie czy ostatnia komórka z listy to koniec labiryntu, żeby oznaczyć to na grafie */
        first_cell = get_from_cells_list( graph.cor_arr[i].cells_list, graph.cor_arr[i].cells_amount-1 );
        if( first_cell == graph.maze -> end_cell ) {
            printf( "%*s(%2d, %2d) ----------->  WYJŚCIE\n\n",
                   3*(level+1),
                   "",
                   
                   first_cell->x,
                   first_cell->y );
        }
        /* przerwszy pomiędzy kolejnymi ścieżkami */
        else if( i+1 < graph.cor_amount && level >= graph.cor_arr[i+1].level ) {
            printf( "\n" );
        }
        if( i+1 < graph.cor_amount && level > graph.cor_arr[i+1].level ) {
            printf( "\n" );
        }
    }
}

void free_graph( graph_t graph ) {
    int i=0;
    
    /* czyszczenie macierzy sąsiedztwa */
    for( i=0; i < graph.maze->rows * graph.maze->cols; ++i ) {
        free( graph.adjacency_matrix[i] );
    }
    free( graph.adjacency_matrix );
    
    /* czyszczenie korytarzy */
    for( i=0; i < graph.cor_amount; ++i ) {
        clear_cells_list( graph.cor_arr[i].cells_list );
    }
    free( graph.cor_arr );
    
    /* czyszczenie dróg */
    clear_paths_list( &(graph.paths_list) );
}
