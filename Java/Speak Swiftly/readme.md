# Speak Swiftly
Speak Swifly to aplikacja służąca do nauki języka angielskiego na podstawie fiszek tworzonych przez użytkownika.

## Sposób użycia
Aby zacząć używać aplikacji wystarczy pobrać plik `Speak Swiftly.jar`, a następnie go otworzyć w przeglądarce plików.
Jeśli chce się go otworzyć z konsoli, można wejść w konsolę, nawigować do folderu z plikiem i użyć komendy:
```sh
java -jar "Speak Swiftly.jar"
```

## Funkcjonalność
### Fiszki
- [x] Tworzenie nowych fiszek
- [x] Usuwanie istniejących fiszek
- [x] Przeglądanie i edytowanie fiszek

### Zbiory fiszek
- [x] Dodawanie nowych zbiorów fiszek
- [x] Usuwanie istniejących zbiorów fiszek
- [x] Przeglądanie i edytowanie zbiorów fiszek

### Nauka
- [x] Możliwość rozpoczęcia testu z fiszek z danej kategorii
- [x] Wybór tłumaczenia z angielskiego na polski lub z polskiego na angielski

## Repozytorium
Repozytorium zawiera kod źródłowy programu napisany w Javie wraz z testami jednostkowymi.
Poza podstawowymi bibliotekami z Javy użyta jest biblioteka `javax.swing` do wykonania interfejsu graficznego.
Pliki znajdują się w tak ułożonych folderach:
```
Speak Swiftly
├───.idea
├───assets
│   ├───fonts
│   └───images
├───out
│   └───production
│       └───Speak Swiftly
└───src
    ├───common
    ├───GUI
    │   ├───buttons
    │   └───panels
    └───tests
```
Gdzie folder `assets` zawiera części graficzne interfejsu, a foldery w `src`:
| Nazwa Folderu  | Zawartość |
|:-------------:|:-------------:|
| common | Pliki klas dla działania wewnętrznego |
| GUI | Pliki klas dla interfejsu graficznego |
| tests | Testy jednostkowe |

## Autorzy
Autorami aplikacji są:
- Chamera Szymon
- Ciuraba Mikołaj
- Wylot Adam
- Żebrowski Jakub