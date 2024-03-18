// NAGŁÓWKI
#include "repository.h"

//                             === MAIN ===
int main( int argc, char** argv ) {
// INICJALIZACJA
    init_rep( argc, argv );
// PROGRAM
    // rysowanie
    print_all();

// ZAMYKANIE PROGRAMU
    free_all();
    return 0; // sukces
}
