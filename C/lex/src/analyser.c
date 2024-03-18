// NAGŁÓWKK
#include "analyser.h"
#include "repository.h"
#include "fun_stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINICJE FUNKCJI
void strrev( char* str ) { // funkcja służąca do odwracania łańcucha znaków
    if( str == NULL ) {
        return;
    }

    int i = 0;
    int j = (int) strlen( str ) - 1;
    char c;

    while( i < j ) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
        ++i; --j;
    }
}


bool analyse_file( analysed_file_t analysed_file ) {
// WCZYTANIE PLIKU
    if( analysed_file->file != NULL ) { // zabezpieczenie
        fclose( analysed_file->file );
    }

    analysed_file->file = fopen( analysed_file->path, "r" );
    if( analysed_file->file == NULL ) {
        char tmp_str[BUFSIZE];
        sprintf( tmp_str, "nie udało się otworzyć pliku o nazwie \"%s\"", analysed_file->path );
        push_error( tmp_str );
        return EXIT_FAILURE;
    }

// ZMIENNE
    char word[BUFSIZE];
    char fun_name[BUFSIZE];
    int ch;
    int next_ch;

    int bracket_lvl = 0;
    int brace_lvl = 0;
    int start_line = 0;
    unsigned lenght = 0;

    fun_stack_t funstack = NULL;
    fun_stack_t fun_pos_stack = NULL;

    bool comment = false;
    bool quote = false;
    bool apostrophe = false;

    // wyzerowanie słów
    word[0] = '\0';
    fun_name[0] = '\0';

    analysed_file->line = 1;

// CZYTANIE PLIKÓW
    ch = get_next_char( analysed_file, word );
    while( ch != EOF ) {
        lenght = strlen( word ); // ustawia długość aktualnie badanego słowa

// obsługa komentarzy
        if( comment ) {
            if( (char)ch == '*' ) {
                next_ch = get_next_char(analysed_file, word);
                if( next_ch == EOF ) {
                    break;
                }

                if( (char)next_ch == '/' && strlen(word) > 1) { // to musi być koniec komentarza
                    comment = false;
                }
            }

            ch = get_next_char( analysed_file, word );
            continue;
        }

// obsługa cudzysłowia
        if( quote ) {
            if( (char)ch == '\"' ) { // koniec cudzysłowia
                quote = false;

                if( lenght > 1 && word[ lenght-2 ] == '\\' ) { // jednak nie koniec
                    quote = true;

                    if( lenght > 2 && word[ lenght-3 ] == '\\' ) { // ostatecznie koniec
                        quote = false;
                    }
                }
            }

            ch = get_next_char( analysed_file, word );
            continue;
        }

// obsługa apostrofu
        if( apostrophe ) {
            if( (char)ch == '\'' ) { // koniec apostrofu
                apostrophe = false;

                if( lenght > 1 && word[ lenght-2 ] == '\\' ) { // jednak nie koniec
                    apostrophe = true;

                    if( lenght > 2 && word[ lenght-3 ] == '\\' ) { // ostatecznie koniec
                        apostrophe = false;
                    }
                }
            }

            ch = get_next_char( analysed_file, word );
            continue;
        }

// ANALIZA WCZYTANEGO ZNAKU
        ///printf( "%c\n", ch );
        switch( (char)ch ) {
        // nawiasy
            case '(': {
                ++bracket_lvl;

                fun_name[0] = '\0';
                int counter=0;
                for( int i=lenght-2; i >= 0; --i ) {
                    if( ( word[i] < 'a' || word[i] > 'z' ) && ( word[i] < 'A' || word[i] > 'Z' ) && word[i] != '_' ) { // nie jest znakiem określającym nazwę funkcji
                        break;
                    }
                    fun_name[ lenght-2-i ] = word[i];
                    ++counter;
                }
                lenght = counter;
                fun_name[ lenght ] = '\0';
                strrev( fun_name );

                // sprawdzanie, czy znaleziona nazwa spełnia warunki bycia funkcją
                if( lenght < 1 || check_in_blacklist( analysed_file->blacklist, fun_name ) ) { // nawias występuje samotnie lub funkcja jest na czarnej liście
                    break;
                }

                // jeżeli nie było jeszcze takiej funkcji to doda ją do tablicy
                if( !check_in_fun_arr( fun_name ) ) {
                    add_fun( analysed_file, fun_name );
                }
                // włożenie na stos tego co zebraliśmy
                fun_stack_push( &funstack, get_fun( fun_name )->name, analysed_file->line );

                break;
            }

            case ')': { // zamknięcie nawiasu, czyli określenie funkcji
                --bracket_lvl;

                // sprawdzenie poprawności składni
                if( bracket_lvl < 0 ) { // zła składnia!
                    char tmp[BUFSIZE];
                    sprintf( tmp, "niepoprawna składnia w linii %d: \"%s\"", analysed_file->line, word );
                    push_error( tmp );

                    bracket_lvl = 0;
                    break;
                }

                // Wpisanie funkcji, jeżeli nie są wpisane
                if( fun_stack_is_empty( funstack ) ) { // zabezpieczenie
                    break;
                }

                // pobranie odpowiadającej temu nawiasowi funkcji ze stosu
                start_line = fun_stack_line_value( funstack );
                char* tmp_str = fun_stack_get( &funstack );
                if( tmp_str == NULL ) { // zabezpieczenie
                    break;
                }

                if( start_line < 0 ) { // zabezpieczenie
                    char tmp[BUFSIZE];
                    sprintf( tmp, "wystąpił nieznany błąd w linii %d", analysed_file->line );
                    push_error( tmp );

                    break;
                }

                // przywrócenie nazwy i rozeznanie
                strncpy( fun_name, tmp_str, BUFSIZE-1 );
                fun_name[ strlen( tmp_str ) ] = '\0';

            // rozpoznawanie typów
                if( brace_lvl > 0 ) { // najpewniej użycie
                    if( !fun_stack_is_empty( fun_pos_stack ) ) { // jeżeli znajduję się w środku innej funkcji to dodaje do niej użycie
                        fun_t tmp_fun = get_fun( fun_stack_seek( fun_pos_stack ) );
                        add_call( tmp_fun, fun_name );
                    }

                    add_usage( get_fun( fun_name ), analysed_file->path, start_line, analysed_file->line );
                    break;
                }

                // skoro nieużycie to szuka następnego znaku, aby określić rodzaj
                next_ch = get_next_char( analysed_file, word );
                if( next_ch == EOF ) { // doszło do końca pliku - błąd (ale zapewne skoro to nie użycie, to najpewniej prototyp)
                    /* char tmp[BUFSIZE];
                    sprintf( tmp, "prawdopodobny błąd składni w linii %d - możliwy brak znaku \';\'", analysed_file->line);
                    push_error( tmp ); */

                    add_prot( get_fun( fun_name ), analysed_file->path, start_line, analysed_file->line );
                    break;
                }

                // cofnięcie programu do stanu przed wczytaniem kolejnego znaku
                (void) fseek( analysed_file->file, -1, SEEK_CUR);

            // wybieranie pomiędzy definicją a prototypem
                if( (char)next_ch == '{' ) { // definicja funkcji
                    fun_stack_push( &fun_pos_stack, get_fun( fun_name )->name, analysed_file->line );
                    add_def( get_fun( fun_name ), analysed_file->path, start_line, analysed_file->line );
                }
                else { // w takim razie pozostał tylko prototyp
                    add_prot( get_fun( fun_name ), analysed_file->path, start_line, analysed_file->line );
                }

                break;
            }

            case '{': {
                ++brace_lvl;
                break;
            }

            case '}': {
                --brace_lvl;
                if( brace_lvl < 0 ) { // zła składnia!
                    char tmp[BUFSIZE];
                    sprintf( tmp, "niepoprawna składnia w linii %d: \"%s\"", analysed_file->line, word );
                    push_error( tmp );

                    brace_lvl = 0;
                    break;
                }

                // zmiana wartości końcowej linii
                if( fun_stack_is_empty( fun_pos_stack ) || brace_lvl != 0 ) { // zabezpieczenie
                    break;
                }

                // pobranie nazwy definicji, w której się aktualnie znajduje
                char* tmp_str = fun_stack_get( &fun_pos_stack );
                if( tmp_str == NULL ) { // zabezpieczenie
                    break;
                }

                // przypisanie wartości do tej definicji
                strncpy( fun_name, tmp_str, BUFSIZE-1 );
                fun_t tmp_fun = get_fun( fun_name );
                tmp_fun->definition->end = analysed_file->line;

                break;
            }

        // znaki specjalne
            case '#': {
                if( lenght > 1 ) { // przed tym muszą być znaki - nie ma definicji ani include
                    break;
                }

                skip_to_space( analysed_file, word );

                // badanie wykrytych słów
                if( strcmp( word, "#define" ) == 0 ) { // definicja stałej, a może nawet makra
                    // teraz funkcja pobierze znak, aby zapoczątkować tworzenie nowego słowa
                    if( get_next_char( analysed_file, word ) == EOF ) {
                        break;
                    }

                    do { // pobiera znaki aż do napotkania początku makra
                        ch = get_next_char( analysed_file, word );
                        if( ch == EOF ) {
                            break;
                        }

                        lenght = strlen( word );
                        if( lenght < 2 ) { // napotkało przerwę, więc cofa to co pobrało za dużo
                            (void) fseek( analysed_file->file, -1, SEEK_CUR );
                            break;
                        }

                    } while( (char)ch != '(' );

                    if( lenght >= 2 ) {
                        //znalazło funkcję, więc doda ją do lokalnej czarnej listy
                        char tmp_str[BUFSIZE];
                        strncpy( tmp_str, word, lenght-1 );
                        tmp_str[ lenght-1 ] = '\0';
                        add_to_blacklist( analysed_file->blacklist, tmp_str );
                    }
                }
                else if( strcmp( word, "#include" ) == 0 ) {
                    skip_to_space( analysed_file, word );
                    lenght = strlen( word );

                    // sprawdzenie formatu #include
                    if( lenght < 3 || word[0] != '\"' || word[ lenght-1 ] != '\"' ) { // niezgodne z formatem
                        break;
                    }

                    // pobranie nazwy funkcji, która jest w cudzysłowie
                    for( unsigned i=1; i < lenght-1; ++i ) {
                        fun_name[i-1] = word[i];
                    }
                    fun_name[ lenght-2 ] = '\0';

                    // jeżeli funkcja istnieje (została już przeanalizowana) to kopiuje jej czarną listę
                    int idx = is_file_exist( fun_name );
                    if( idx != -1 ) {
                        blacklist_copy( analysed_file->blacklist, repository.files_arr[idx]->blacklist );
                        skip_line( analysed_file );
                        break;
                    }

                    /* czytanie pliku dla funkcji wczytanej w include oraz skopiowanie czarnej listy tego pliku,
                     * czyli wszystkie makra wczytane za pomocą #define, będą pomijane również w tym pliku       */
                    idx = load_file( fun_name );
                    if( idx >= 0 ) {
                        blacklist_copy( analysed_file->blacklist, repository.files_arr[idx]->blacklist );
                    }
                }

                skip_line( analysed_file );
                break;
            }

            case '/': {
                next_ch = get_next_char( analysed_file, word );
                if( next_ch == EOF ) { // zabezpieczenie
                    break;
                }

                if( (char)next_ch == '/' && strlen( word ) > 1 ) { // następny znak to też '/', także to jest komentarz
                    skip_line( analysed_file );
                }
                else if( (char)next_ch == '*' && strlen( word ) > 1 ) { // następny znak to '*', więc to komentarz
                    comment = true;
                }
                else { // żadne z powyższych, więc cofa pobranie tego znaku, aby go ponownie przeanalizować w następnej iteracji
                    (void) fseek( analysed_file->file, -1, SEEK_CUR );
                }

                break;
            }

            case '\"': { // prawdopodobnie start słów w cudzysłowie
                if( lenght > 1 && word[ lenght-2 ] == '\\' ) { // jednak nie
                    break;
                }

                quote = true;
                break;
            }

            case '\'': { // prawdopodobnie start opisywania znaku (typ char) w apostrofie
                if( lenght > 1 && word[ lenght-2 ] == '\\' ) { // jednak nie
                    break;
                }

                apostrophe = true;
                break;
            }
        }

// CZYNNOŚCI KOŃCZĄCE POJEDYŃCZĄ ITERACJE
        ch = get_next_char( analysed_file, word );
    }


// KONIEC ANALIZOWANIA PLIKU
    // zła składnia!
    if( brace_lvl != 0 || bracket_lvl != 0 ) {
        char tmp_str[BUFSIZE];
        sprintf( tmp_str, "niepoprawna składnia w pliku \"%s\"", analysed_file->path );
        push_error( tmp_str );
    }

    // czyszczenie stosów (a raczej upewnienie się, że są puste)
    fun_stack_clear( &funstack );
    fun_stack_clear( &fun_pos_stack );

    // informacja o zamykaniu pliku
    char str[BUFSIZE];
    sprintf( str, "zakończono czytanie pliku \"%s\"", analysed_file->path );
    send_message( str );

    fclose( analysed_file->file );
    analysed_file->file = NULL;

    // sprawdzanie końcówki wczytanej nazwy - jeżeli plik jest w formacie *.h to czyta też jego odpowiednik *.c
    strcpy( fun_name, analysed_file->path );
    lenght = strlen( fun_name );

    if( lenght >= 3 && fun_name[ lenght-1 ] == 'h' && fun_name[ lenght-2 ] ==  '.' ) {
        fun_name[ lenght-1 ] = 'c';

        // powtórzenie czynności wczytywania pliku
        (void) load_file( fun_name );
    }

    return EXIT_SUCCESS;
}

int get_next_char( analysed_file_t analysed_file, char* word ) {
    int x = fgetc( analysed_file->file );

    if( x <= ' ' || x >= 127 ) { // przerywnik pomiędzy literami - kolejne słowo, a więc zeruje wskaźnik na poprzednie
        word[0] = '\0';
    }

    /* Spacja to ostatni z "pustych" znaków, a znak o numerze 127 jest swojego rodzaju zabezpieczeniem,
     * aby upewnić się, że znak nie jest spoza tablicy znaków ASCII (oraz nie jest znakiem DEL - 127. numer) */
    while( x <= ' ' || x >= 127 ) {
        // sprawdzanie, czy nie doszło do końca pliku
        if( x == EOF ) {
            return EOF;
        }

        // liczenie linii
        if( x == '\n' ) {
            ++(analysed_file->line);
        }

        x = fgetc( analysed_file->file );
    }

    // tworzenie ciągu znaków
    char ch = (char)x;
    strncat( word, &ch, 1 );

    return x;
}

void skip_to_space( analysed_file_t analysed_file, char* word ) {
    int x = fgetc( analysed_file->file );

    if( x <= ' ' || x >= 127 ) { // przerywnik pomiędzy literami - kolejne słowo, a więc zeruje wskaźnik na poprzednie
        word[0] = '\0';
    }

// POMINIĘCIE PUSTYCH MIEJSC
    /* Spacja to ostatni z "pustych" znaków, a znak o numerze 127 jest swojego rodzaju zabezpieczeniem,
     * aby upewnić się, że znak nie jest spoza talbicy znaków ASCII (oraz nie jest znakiem DEL - 127. numer) */
    while( x <= ' ' || x >= 127 ) {
        // sprawdzanie, czy nie doszło do końca pliku
        if( x == EOF ) {
            return;
        }

        // liczenie linii
        if( x == '\n' ) {
            ++(analysed_file->line);
        }

        x = fgetc( analysed_file->file );
    }

    // DOPISANIE LITER AŻ DO SPACJI
    char ch;
    do {
        ch = (char)x;
        strncat( word, &ch, 1 );

        x = fgetc( analysed_file->file );
        if( x == EOF) {
            return;
        }
    } while( x > ' ' && x < 127 );

    fseek( analysed_file->file, -1, SEEK_CUR ); // zawsze przeczyta o jeden znak za dużo, więc cofa plik o jeden bajt (char)
}

void skip_line( analysed_file_t analysed_file ) {
    fscanf( analysed_file->file, "%*[^\n]" );
}
