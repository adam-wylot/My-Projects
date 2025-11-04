package common.fileHandlers;

import common.Cell;
import common.Maze;
import common.Settings;
import common.Validator;
import exceptions.InvalidFileException;
import exceptions.NoSuchCellException;
import interfaces.IFileHandler;
import numerators.Direction;

import javax.swing.*;
import java.io.*;


public class TextFH implements IFileHandler {
// Zmienne
    private final String path;
    private final int width;
    private final int height;
    private final char[][] map;     /// [x][y]


// Konstruktory
    public TextFH(String path) throws IOException, InvalidFileException {
        this.path = path;

        width = readWidth();
        height = readHeight();
        map = new char[width][height];

        readFile();
        Validator.isValid(map);
    }


// Gettery
    @Override public int getWidth() { return (width - 1) / 2; }
    @Override public int getHeight() { return (height - 1) / 2; }


// Metody
    public static boolean isTextFile(String path) throws IOException {
    // próba odczytania czterech ostatnich znaków
        if (path.length() >= 4) {
            String last4letters = path.substring(path.length() - 4);

            if (last4letters.equals(".txt")) {
                return true;
            }
        }

    // Jeżeli się nie udało, to otwiera plik i sprawdza

        try (FileReader fr = new FileReader(path)) {
            int ch = 0;
            if ((ch = fr.read()) != -1 && (char) ch == Settings.WALL_CHAR) {
                // udało się przeczytać znak i jest to narożnik labiryntu
                fr.close();
                return true;
            }
        } catch (IOException e) {
            throw new IOException(e.getMessage());
        }


        return false;
    }

    public static void saveMaze(Maze maze, String filePath, boolean doWritePath) {
        if (maze == null)
            return;

        // utworzenie buffera
        char[][] buffer = new char[2*maze.getWidth() + 1][2*maze.getHeight() + 1];


        for (int y = 0; y < 2*maze.getHeight() + 1; y++) {
            for (int x = 0; x < 2*maze.getWidth() + 1; x++) {
                buffer[x][y] = (char) Settings.WALL_CHAR;
            }
        }

        // przeczytanie komórek
        Cell cell;

        for (int y = 0; y < maze.getHeight(); y++) {
            for (int x = 0; x < maze.getWidth(); x++) {
                buffer[2*x + 1][2*y + 1] = (char) Settings.PASS_CHAR;
                cell = maze.getCellAt(x, y);

                if (cell.isWallOpen(Direction.Up)) {
                    buffer[2*x + 1][2*y] = (char) Settings.PASS_CHAR;
                }
                if (cell.isWallOpen(Direction.Right)) {
                    buffer[2*x + 2][2*y + 1] = (char) Settings.PASS_CHAR;
                }
                if (cell.isWallOpen(Direction.Down)) {
                    buffer[2*x + 1][2*y + 2] = (char) Settings.PASS_CHAR;
                }
                if (cell.isWallOpen(Direction.Left)) {
                    buffer[2*x][2*y + 1] = (char) Settings.PASS_CHAR;
                }
            }
        }


        // zapis ścieżki
        if (doWritePath && maze.getPath() != null) {
            Cell prevCell;

            for (int i = 1; i < maze.getPath().size(); ++i) {
                prevCell = maze.getPath().get(i - 1);
                cell = maze.getPath().get(i);

                buffer[2*cell.getX() + 1][2*cell.getY() + 1] = (char) Settings.PATH_CHAR;   // gwiazdkowanie komórki

                // gwiazdkowanie przejścia
                if (cell.getY() - prevCell.getY() == -1) {
                    // przejście w górę — gwiazdkuję dolną ścianę
                    buffer[2*cell.getX() + 1][2*cell.getY() + 2] = (char) Settings.PATH_CHAR;
                }
                else if (cell.getX() - prevCell.getX() == 1) {
                    // przejście w prawo — gwiazdkuję lewą ścianę
                    buffer[2*cell.getX()][2*cell.getY() + 1] = (char) Settings.PATH_CHAR;
                }
                else if (cell.getY() - prevCell.getY() == 1) {
                    // przejście w dół — gwiazdkuję górną ścianę
                    buffer[2*cell.getX() + 1][2*cell.getY()] = (char) Settings.PATH_CHAR;
                }
                else if (cell.getX() - prevCell.getX() == -1) {
                    // przejście w lewo — gwiazdkuję prawą ścianę
                    buffer[2*cell.getX() + 2][2*cell.getY() + 1] = (char) Settings.PATH_CHAR;
                }
            }
        }


        // przeczytanie wejścia i wyjścia
        cell = maze.getStartCell();
        buffer[2*cell.getX() + 1][2*cell.getY() + 1] = (char) Settings.ENTRY_CHAR;

        cell = maze.getEndCell();
        buffer[2*cell.getX() + 1][2*cell.getY() + 1] = (char) Settings.EXIT_CHAR;


        // zapis buffera do pliku
        try (FileWriter file = new FileWriter(filePath)) {
            for (int y = 0; y < 2*maze.getHeight() + 1; y++) {
                for (int x = 0; x < 2*maze.getWidth() + 1; x++) {
                    file.write(buffer[x][y]);
                }
                file.write(System.lineSeparator()); // nowa linia
            }

        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, "Nie udało się zapisać pliku tekstowego!" + e.getMessage(), "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }

        System.out.println("Pomyślnie zapisano plik do: " + filePath + ".");
    }



    private int readWidth() throws IOException {
        String line;

        try (BufferedReader bufferedReader = new BufferedReader(new FileReader(path))) {
            if (!bufferedReader.ready()) {
                throw new IOException("\n\nNie udało się otworzyć pliku");
            }
            if ((line = bufferedReader.readLine()) == null) {
                throw new IOException("\n\nNie udało się przeczytać szerokości labiryntu");
            }
        } catch (IOException e) {
            throw new IOException("\n\nNie udało się przeczytać szerokości labiryntu");
        }

        return line.length();
    }

    private int readHeight() throws IOException {
        int counter = 0;

        try (BufferedReader bufferedReader = new BufferedReader(new FileReader(path))) {
            if (!bufferedReader.ready()) {
                throw new IOException("\n\nNie udało się otworzyć pliku");
            }
            while (bufferedReader.readLine() != null) {
                counter++;
            }
        } catch (IOException e) {
            throw new IOException("\n\nNie udało się przeczytać wysokości labiryntu");
        }

        return counter;
    }

    private void readFile() throws IOException, InvalidFileException {
        try (BufferedReader bufferedReader = new BufferedReader(new FileReader(path))) {
            String readLine;
            int y = 0;
            char c = 0;

            // czytanie pliku
            while ((readLine = bufferedReader.readLine()) != null) {
                if (readLine.length() != width) {
                    throw new InvalidFileException("\n\nLabirynt ma nierówną szerokość");
                }

                for (int x = 0; x < readLine.length(); x++) {
                    c = readLine.charAt(x);
                    if (c == (char) Settings.PATH_CHAR)
                        c = (char) Settings.PASS_CHAR;

                    map[x][y] = c;
                }

                y++;
            }
        } catch (IOException e) {
            throw new IOException(e.getMessage());
        }
    }


    // Metody interfejsu IFileHandler
    @Override
    public int getStartCellX() throws NoSuchCellException {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[x][y] == 0) {
                    throw new NoSuchCellException();
                }

                if (map[x][y] == Settings.ENTRY_CHAR) {
                    // znaleziono znak wejścia do labiryntu, więc rozpoczynam analizę tego, która komórka to wejście

                    if (x == 0) {
                        // lewa ściana
                        return x;
                    }

                    if (x == width - 1) {
                        // prawa ściana
                        return x / 2 - 1;
                    }

                    return (x - 1) / 2;     // środek labiryntu
                }
            }
        }

        throw new NoSuchCellException();
    }

    @Override
    public int getStartCellY() throws NoSuchCellException {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[x][y] == 0) {
                    throw new NoSuchCellException();
                }

                if (map[x][y] == Settings.ENTRY_CHAR) {
                    // znaleziono znak wejścia do labiryntu, więc rozpoczynam analizę tego, która komórka to wejście

                    if (y == 0) {
                        // górna ściana
                        return y;
                    }

                    if (y == height - 1) {
                        // dolna ściana
                        return y / 2 - 1;
                    }

                    return (y - 1) / 2;     // środek labiryntu
                }
            }
        }

        throw new NoSuchCellException();
    }

    @Override
    public int getEndCellX() throws NoSuchCellException {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[x][y] == 0) {
                    throw new NoSuchCellException();
                }

                if (map[x][y] == Settings.EXIT_CHAR) {
                    // znaleziono znak wyjścia labiryntu, więc rozpoczynam analizę tego, która komórka to wyjście

                    if (x == 0) {
                        // lewa ściana
                        return x;
                    }

                    if (x == width - 1) {
                        // prawa ściana
                        return x / 2 - 1;
                    }

                    return (x - 1) / 2;     // środek labiryntu
                }
            }
        }

        throw new NoSuchCellException();
    }

    @Override
    public int getEndCellY() throws NoSuchCellException {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[x][y] == 0) {
                    throw new NoSuchCellException();
                }

                if (map[x][y] == Settings.EXIT_CHAR) {
                    // znaleziono znak wyjścia labiryntu, więc rozpoczynam analizę tego, która komórka to wyjście

                    if (y == 0) {
                        // górna ściana
                        return y;
                    }

                    if (y == height - 1) {
                        // dolna ściana
                        return y / 2 - 1;
                    }

                    return (y - 1) / 2;     // środek labiryntu
                }
            }
        }

        throw new NoSuchCellException();
    }

    @Override
    public Cell getCellAt(int x, int y) throws NoSuchCellException {
        int mapX = 2*x + 1;
        int mapY = 2*y + 1;

        // sprawdzenie istnienia komórki
        if (map[mapX][mapY] == 0) {
            throw new NoSuchCellException();
        }

        // utworzenie komórki
        Cell cell = new Cell(x, y);

        if (map[mapX][mapY - 1] == Settings.PASS_CHAR) {
            // górna ściana
            cell.setPass(true, Direction.Up);
        }

        if (map[mapX + 1][mapY] == Settings.PASS_CHAR) {
            // prawa ściana
            cell.setPass(true, Direction.Right);
        }

        if (map[mapX][mapY + 1] == Settings.PASS_CHAR) {
            // dolna ściana
            cell.setPass(true, Direction.Down);
        }

        if (map[mapX - 1][mapY] == Settings.PASS_CHAR) {
            // lewa ściana
            cell.setPass(true, Direction.Left);
        }

        return cell;
    }
}
