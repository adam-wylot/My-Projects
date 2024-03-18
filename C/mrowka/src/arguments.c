#include "arguments.h"
#include "repository.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_arguments( int argc, char** argv ) {
    // niewystarczająca liczba parametrów wywołania
    if( argc < 6 ) {
        char str[BUFSIZE];
        sprintf( str, "niewystarczająca liczba parametrów wywołania - podano %d, a oczekiwano przynajmniej 5", argc-1 );

        push_crit_error( str );
        return;
    }

// WARTOŚCI POCZĄTKOWE
    arguments.rows = 0;
    arguments.cols = 0;
    arguments.iterations = 0;
    arguments.file_prefix = NULL;
    arguments.direction = UP;

    arguments.print_all_flag = false;
    arguments.map_path = NULL;
    arguments.map_flag = false;
    arguments.percentage = 0;
    arguments.perc_flag = false;

// WCZYTYWANIE ARGUMENTÓW PODSTAWOWYCH
    read_required_args( argv );

// WCZYTYWANIE ARGUMENTÓW OPCJONALNYCH
    read_additional_args( argc, argv );

// SPRAWDZENIE POPRAWNOŚCI DANYCH
    check_args();
}

void read_required_args( char** argv ) {
    arguments.rows = atoi( argv[1] ); // wiersze

    arguments.cols = atoi( argv[2] ); // kolumny

    // liczba iteracji
    /// w przypadku nieudanej konwersji wyrzuci 0, więc program wyrysuje stan początkowy
    arguments.iterations = atoi( argv[3] );

    arguments.file_prefix = argv[4]; // prefix do plików zapisu iteracji

    arguments.direction = (direction_t) atoi( argv[5] );    // początkowy kierunek mrówki
    /// jeżeli nie uda się przekonwertować parametru na liczbę to automatycznie ustawia wartość na 0 (czyli górę [UP == 0] )
}

void read_additional_args( int argc, char** argv ) {
// WARUNKI
    if( argc <= 0 ) { // brak argumentów
        return;
    }

// CZYTANIE
    optind = 6;
    int opt=0;
    /** -a                  | rysuje w konsoli wszystkie iteracje
      * -m (nazwa pliku)    | wgrywa gotową mapę
      * -p (liczba)         | % początkowego zapełnienia mapy
      * */

    while( (opt = getopt( argc, argv, "am:p:" )) != -1 ) {
        switch( opt ) {
            case 'a': {
                if( !arguments.print_all_flag ) {
                    arguments.print_all_flag = true;
                    send_message( "ustawiono rysowanie wszystkich iteracji" );
                }
                else {
                    push_error( "wystąpiło ponownie użycie opcji -a" );
                }

                break;
            }

            case 'm': {
                if( !arguments.map_flag ) { // flaga nie wystąpiła
                    arguments.map_flag = true;

                    if( access( optarg, F_OK ) == 0 ) { // plik istnieje
                        arguments.map_path = optarg;
                        send_message( "pomyślnie wczytano stan początkowy siatki" );
                    }
                    else { // plik nie istnieje lub nie udało się go otworzyć
                        char str[BUFSIZE];
                        sprintf( str, "nie wczytano mapy - nie udało się otworzyć pliku \"%s\"", optarg );
                        push_error( str );
                    }
                }
                else {
                    push_error( "wystąpiło ponownie użycie opcji -m" );
                }

                break;
            }


            case 'p': {
                if( !arguments.perc_flag ) { // flaga nie wystąpiła
                    arguments.perc_flag = true;

                    arguments.percentage = atoi( optarg );
                    if( arguments.percentage > 0 && arguments.percentage <= 100 ) { // podano dobrą wartość
                        send_message( "pomyślnie wczytano początkowe wypełnienie siatki" );
                    }
                    else { // błąd konwersji łańcucha lub zła wartość
                        arguments.percentage = 0;
                        push_error( "nie udało się wczytać początkowego wypełnienia siatki - błąd konwersji lub błędna wartość" );
                    }
                }
                else {
                    push_error( "wystąpiło ponownie użycie opcji -p" );
                }

                break;
            }


            case '?': {
                if( optopt == 'm' ) {
                    push_error( "opcja -m potrzebuje argumentu" );
                }
                else if( optopt == 'p' ) {
                    push_error( "opcja -p potrzebuje argumentu" );
                }
                else if( optopt == 'a' ) {
                    push_error( "opcja -a nie przyjmuje żadnych argumentów" );
                }
                else {
                    char str[BUFSIZE];
                    sprintf( str, "wczytano nieznaną opcję -%c", optopt );
                    push_error( str );
                }

                break;
            }
            default: {
                push_crit_error( "wystąpił nieznany błąd" );
                break;
            }
        }
    }

    // wypisanie dodatkowych argumentów jako błędy
    char str[BUFSIZE];

    for( int index = optind; index < argc; ++index ) {
        sprintf( str, "otrzymano parametr \"%s\", który nie był wymagany", argv[optind] );
        push_error( str );
    }

    // aktualizacja flag
    if( arguments.percentage == 0 ) {
        arguments.perc_flag = false;
    }
    if( arguments.map_path == NULL ) {
        arguments.map_flag = false;
    }

    // sprawdzenie, czy mapa będzie wczytywana - jeżeli tak to bada i aktualizuje liczbę kolumn i wierszy
    if( arguments.map_flag ) {
        if( arg_read_file( arguments.map_path ) != 0 ) {

            // wystąpił błąd, więc:
            sprintf( str, "nie udało się wczytać mapy \"%s\" - wykonuję program dla pustej siatki", arguments.map_path );
            push_error( str );

            arguments.map_flag = false;
        }
    }
}

void check_args( void ) {
    // wiersze
    if( arguments.rows < 5 ) { // błąd konwersji lub za mała wartość
        push_crit_error( "nie udało się wczytać liczby wierszy - błąd konwersji lub za mała liczba (min. 5)" );
        return;
    }

    // kolumny
    if( arguments.cols < 5 ) { // błąd konwersji lub za mała wartość
        push_crit_error( "nie udało się wczytać liczby kolumn - błąd konwersji lub za mała liczba (min. 5)" );
        return;
    }

    // początkowy kierunek mrówki
    if( arguments.direction < 0 || arguments.direction > 3 ) { // zabezpieczenie
        arguments.direction = 0; // UP
    }
}

int arg_read_file( char* path ) {
    FILE* file = fopen( path, "r" );
    if( file == NULL ) { // nie udało się otworzyć pliku
        char str[BUFSIZE];
        sprintf( str, "nie udało się otworzyć pliku \"%s\" do jego odczytu", path );

        push_error( str );
        return 1; /// porażka
    }

    int rows=1, cols=0, c=0;

    // liczenie kolumn
    while( getc( file ) != '\n' ) {
        ++cols;
    }
    cols /= 3; // 3 bajty to jeden znak dlatego zmniejszam liczbę kolumn trzykrotnie
    cols = (cols - 1) / 2;

    // liczenie wierszy
    while( (c = getc( file )) != EOF ) {
        if( c == '\n' ) {
            ++rows;
        }
    }
    rows = (rows - 1) / 2;

    // zapobieganie błędom
    if( rows < 5 ) {
        char str[BUFSIZE];
        sprintf( str, "wczytana mapa ma za mało wierszy - wczytano %d, a wymagano minimalnie 5", rows );

        push_error( str );
        fclose( file );
        return 2; /// porażka
    }
    if( cols < 5 ) {
        char str[BUFSIZE];
        sprintf( str, "wczytana mapa ma za mało kolumn - wczytano %d, a wymagano minimalnie 5", cols );

        push_error( str );
        fclose( file );
        return 3; /// porażka
    }

    arguments.rows = rows;
    arguments.cols = cols;

    fclose( file );
    return 0; /// sukces
}
