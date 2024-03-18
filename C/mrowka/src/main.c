#include "net.h"
#include "repository.h"
#include "arguments.h"
#include "filer.h"

/** wiersze, kolumny, liczba iteracji, przedrostek (str), początkowy kierunek mrówki, +opcjonalne wczytanie mapy,
  * +opcjonalne wygenerowanie mapy (na podstawie % zapełnienia) , +opcjonalne rysowanie mapy w czasie rzeczywistym    */
int main( int argc, char** argv ) {

// Startowanie programu
    init_rep( argv[0] );
    init_arguments( argc, argv );

// Program
    net_t net = net_init( arguments.rows, arguments.cols, arguments.direction );

    // implementowanie opcjonalnych parametrów wywołania
    if( arguments.map_flag ) { // mapa początkowa
        file_load( net->gui, arguments.map_path );
        net_set_from_gui( net );
    }
    else if( arguments.perc_flag ) { // procentowe wypełnienie
        net_gen_by_perc( net, arguments.percentage );
    }

    // wykonywanie ruchów mrówki
    net_make_moves( net, arguments.iterations );

// Kończenie programu
    net_free( &net );

    return 0; /// sukces
}
