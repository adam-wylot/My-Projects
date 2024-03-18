=== DZIAŁANIE: ===
Program lex rozpoznaje funkcje oraz ich użycie w kodzie.

Do rozpoznania funkcji czyta pliki, które użytkownik podaje jako argumenty przy wywołaniu programu.

Nowo rozpoznane nazwy funkcji zapisuje w strukturze danych chyba, że ich nazwa znajduje się na czarnej liście nazw funkcji.
Następnie program rozpoznaje, czy w danym miejscu w poszczególnym pliku występuje prototyp, definicja, czy wywołanie funkcji. Te informacje również zapisuje w strukturze danych.

Program dodatkowo liczy zbilansowanie nawiasowe i jeżeli występują niezgodności to informuje użytkownika o błędach w składni.



=== CZARNA LISTA: ===
Program posiada dwa typy czarnej listy:

Pierwsza z nich to globalna czarna lista. Obowiązuje ona we wszystkich plikach, które będzie analizował program. Znajduje się ona w w pliku "blacklist". Nazwy funkcji są oddzielone pomiędzy sobą spacjami.

Drugim typem czarnej listy jest lokalna czarna lista. Obowiązuję ona wyłącznie w analizowanym aktualnie pliku. Jest ona na bierząco aktualizowana np. podczas wystąpienia makra.
Jeżeli czytany plik wczytuje inny plik za pomocą formuły "#include", to lokalna czarna lista wczytanego pliku jest dziedziczona przez aktualnie analizowany plik.



=== (#define): ===
Program lex rozpoznaje również definicje makr. Wykrywa, że dana nazwa to nie funkcja tylko makro, a następnie dodaje je do lokalnej czarnej listy. Następne wystąpienia tej nazwy będą pomijane i nieanalizowane przez program.



=== (#include): ===
Analizator leksykalny rozpoznaje użycie słowa "#include". Gdy uda mu się otworzyć plik zawarty w cudzysłowie to najpierw rozpoczyna analizowanie tego pliku, a dopiero później wraca do pliku pierwotnego.



=== PLIKI *.h: ===
Program po ukończeniu czytania pliku, rozpoznaje, czy był to plik z rozszerzeniem *.h - jeżeli tak to próbuje również przeczytać plik o tej samej nazwie, ale z rozszerzeniem *.c.



=== SZCZEGÓŁOWY OPIS DZIAŁANIA: ===
Analizator leksykalny działa za pomocą czytania każdego znaku w pliku (oddzielnie). Jeżeli pomiędzy kolejnymi wczytanymi znakami nie występują przerwy to dodaje je do jednego łańcucha znaków. Wykorzystuje to później do pobrania nazwy funkcji.