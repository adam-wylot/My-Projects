-------------------------------------------{ filecalc }------------------------------------------

Program służy do wykonywania operacji matematycznych jednego typu, które zawarte są w pliku
wejściowym. Dokładny opis programu oraz format pliku wejściowego znajdują się w dokumentacji
projektowej.

Plik wykonywalny znajduje się (oraz jest tworzony przy ponownej kompilacji) w katalogu bin.

=================================================================================================

Użycie: filecalc.exe [-h] <wejście> [wyjście]


        Opcje:
           -h   	| wyświetla podręcznik użytkownika
           <wejście>    | ścieżka do pliku wejściowego (wymagana, o ile nie podano wcześniej -h)
           [wyjście]    | ścieżka do pliku wyjściowego (opcjonalna). Jeśli nie podano
                          wynik zostanie zapisany w "out_(nazwa pliku wejściowego).txt"


        Przykłady:
           filecalc.exe -h
           filecalc.exe input.txt
           filecalc.exe input.txt output.txt

-------------------------------------------------------------------------------------------------