package common;

import exceptions.InvalidFileException;


public class Validator {
    private Validator() {}

    // Metody
    public static int isValid(char[][] map) throws InvalidFileException {
        int width = map.length;
        int height = map[0].length;



    // Otoczenie ścianami
        for (int i = 0; i < width; i++) {
            if (map[i][0] != (char) Settings.WALL_CHAR && map[i][0] != (char) Settings.ENTRY_CHAR && map[i][0] != (char) Settings.EXIT_CHAR) {
                throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być ściana (wiersz: 1, kolumna: " + (i + 1) + ")" );
            }
            if (map[i][height - 1] != (char) Settings.WALL_CHAR && map[i][height - 1] != (char) Settings.ENTRY_CHAR && map[i][height - 1] != (char) Settings.EXIT_CHAR) {
                throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być ściana (wiersz: " + height + ", kolumna: " + (i + 1) + ")" );
            }
        }

        for (int i = 0; i < height; i++) {
            if (map[0][i] != (char) Settings.WALL_CHAR && map[0][i] != (char) Settings.ENTRY_CHAR && map[0][i] != (char) Settings.EXIT_CHAR) {
                throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być ściana (wiersz: " + (i + 1) + ", kolumna: 1)" );
            }
            if (map[width - 1][i] != (char) Settings.WALL_CHAR && map[width - 1][i] != (char) Settings.ENTRY_CHAR && map[width - 1][i] != (char) Settings.EXIT_CHAR) {
                throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być ściana (wiersz: " + (i + 1) + ", kolumna: " + height + ")" );
            }
        }



    // Poprawne wejście/wyjście
        boolean entryFlag = false;
        boolean exitFlag = false;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[x][y] == (char) Settings.ENTRY_CHAR) {
                    if (entryFlag) {
                        throw new InvalidFileException("Znaleziono więcej niż jedno wejście");
                    }

                    // sprawdzenie położenia wejścia
                    if (x == 0 || x == width - 1) {
                        if (y % 2 == 0)
                            throw new InvalidFileException("Wejście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }
                    else if (y == 0 || y == height - 1) {
                        if (x % 2 == 0)
                            throw new InvalidFileException("Wejście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }
                    else {
                        // jest gdzieś w środku
                        if (x % 2 == 0 || y % 2 == 0)
                            throw new InvalidFileException("Wejście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }

                    entryFlag = true;
                }

                else if (map[x][y] == (char) Settings.EXIT_CHAR) {
                    if (exitFlag) {
                        throw new InvalidFileException("Znaleziono więcej niż jedno wyjście");
                    }

                    // sprawdzenie położenia wyjścia
                    if (x == 0 || x == width - 1) {
                        if (y % 2 == 0)
                            throw new InvalidFileException("Wyjście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }
                    else if (y == 0 || y == height - 1) {
                        if (x % 2 == 0)
                            throw new InvalidFileException("Wyjście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }
                    else {
                        // jest gdzieś w środku
                        if (x % 2 == 0 || y % 2 == 0)
                            throw new InvalidFileException("Wyjście do labiryntu jest nieosiągalne lub niejednoznaczne");
                    }

                    exitFlag = true;
                }
            }
        }

        if (entryFlag == false) {
            throw new InvalidFileException("Wprowadzony labirynt nie posiada wejścia");
        }
        if (exitFlag == false) {
            throw new InvalidFileException("Wprowadzony labirynt nie posiada wyjścia");
        }


    // Znaki w labiryncie
        /// nieokreślone znaki
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                if (map[x][y] != (char) Settings.WALL_CHAR && map[x][y] != (char) Settings.PASS_CHAR && map[x][y] != (char) Settings.PATH_CHAR && map[x][y] != (char) Settings.ENTRY_CHAR && map[x][y] != (char) Settings.EXIT_CHAR) {
                    throw new InvalidFileException("Znaleziono nieokreślony znak: \"" + map[x][y] + "\" w wierszu " + (y + 1) + ", kolumnie " + (x + 1));
                }
            }
        }

        // znaki ścian w rogach komórek
        width = (width - 1) / 2;
        height = (height - 1) / 2;

        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                if (map[2*x + 2][2*y + 2] != Settings.WALL_CHAR) {
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być ściana (wiersz: " + (2*y + 3) + ", kolumna: " + (2*x + 3) + ")" );
                }
            }
        }



    // Komórki w labiryncie
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[2*x + 1][2*y + 1] != Settings.PASS_CHAR && map[2*x + 1][2*y + 1] != Settings.PATH_CHAR && map[2*x + 1][2*y + 1] != Settings.ENTRY_CHAR && map[2*x + 1][2*y + 1] != Settings.EXIT_CHAR) {
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinna być komórka (wiersz: " + (2*y + 2) + ", kolumna: " + (2*x + 2) + ")" );
                }


                // Sprawdzenie ścian komórki
                if (y != 0 && map[2*x + 1][2*y] != Settings.PASS_CHAR && map[2*x + 1][2*y] != Settings.WALL_CHAR && map[2*x + 1][2*y] != Settings.PATH_CHAR) {
                    /// GÓRA
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinno być przejście lub ściana komórki (wiersz: " + (2*y + 1) + ", kolumna: " + (2*x + 2) + ")" );
                }
                if (x != width - 1 && map[2*x + 2][2*y + 1] != Settings.PASS_CHAR && map[2*x + 2][2*y + 1] != Settings.WALL_CHAR && map[2*x + 2][2*y + 1] != Settings.PATH_CHAR) {
                    /// PRAWO
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinno być przejście lub ściana komórki (wiersz: " + (2*y + 2) + ", kolumna: " + (2*x + 3) + ")" );
                }
                if (y != height - 1 && map[2*x + 1][2*y + 2] != Settings.PASS_CHAR && map[2*x + 1][2*y + 2] != Settings.WALL_CHAR && map[2*x + 1][2*y + 2] != Settings.PATH_CHAR) {
                    /// DÓŁ
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinno być przejście lub ściana komórki (wiersz: " + (2*y + 3) + ", kolumna: " + (2*x + 2) + ")" );
                }
                if (x != 0 && map[2*x][2*y + 1] != Settings.PASS_CHAR && map[2*x][2*y + 1] != Settings.WALL_CHAR && map[2*x][2*y + 1] != Settings.PATH_CHAR) {
                    /// LEWO
                    throw new InvalidFileException("Nieprawidłowy znak w miejscu, gdzie powinno być przejście lub ściana komórki (wiersz: " + (2*y + 2) + ", kolumna: " + (2*x + 1) + ")" );
                }
            }
        }

        return 0;
    }
}
