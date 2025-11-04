package common.fileHandlers;

import common.*;
import exceptions.InvalidFileException;
import exceptions.NoSuchCellException;
import interfaces.IFileHandler;
import numerators.Direction;

import javax.swing.*;
import java.io.*;
import java.util.ArrayList;


public class BinaryFH implements IFileHandler {
    // Zmienne
    private final String path;
    int head;
    byte escape;
    int width;
    int height;
    short entryX;
    short entryY;
    short exitX;
    short exitY;
    byte[] reserved = new byte[12];
    int counter;
    int solutionOffset;
    byte separator;
    byte word_wall;
    byte word_path;
    char[][] maze_cells;



    // Konstruktory
    public BinaryFH(String path) throws IOException, InvalidFileException {
        this.path = path;
        maze_cells = null;

        readBinaryFile(path);
        Validator.isValid(maze_cells);
    }


// Metody
    public static boolean isBinaryFile(String path) throws IOException {
    // próba odczytania czterech ostatnich znaków
        if (path.length() >= 4) {
            String last4letters = path.substring(path.length() - 4);

            if (last4letters.equals(".bin")) {
                return true;
            }
        }

    // Jeżeli się nie udało, to otwiera plik i sprawdza
        // Plik binarny
        DataInputStream dis = new DataInputStream(new FileInputStream(path));

        if (dis.readInt() == 0x52524243) {
            dis.close();
            return true;
        }

        dis.close();
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

        // Utworzenie zbioru słów kodowych
        int counter = 0;

        ArrayList<Character> symbols = new ArrayList<>();
        ArrayList<Integer> countList = new ArrayList<>();
        char lastChar = 0;

        for (int y = 0; y < 2*maze.getHeight() + 1; y++) {
            for (int x = 0; x < 2 * maze.getWidth() + 1; x++) {
                if (buffer[x][y] == lastChar && counter < 255) {
                    ++counter;
                }
                else {
                    if (x != 0 || y != 0) {
                        symbols.add(lastChar);
                        countList.add(counter);
                    }

                    lastChar = buffer[x][y];
                    counter = 0;
                }
            }
        }
        symbols.add(lastChar);
        countList.add(counter);



        // Utworzenie ścieżki (o ile istnieje)
        ArrayList<Character> directions = new ArrayList<>();
        ArrayList<Integer> steps = new ArrayList<>();
        Cell lastCell;
        int stepsCounter = 0;
        lastChar = 0;
        counter = 0;

        if (doWritePath && maze.getPath() != null) {
            for (int i = 1; i < maze.getPath().size(); i++) {
                lastCell = maze.getPath().get(i - 1);
                cell = maze.getPath().get(i);
                ++stepsCounter;

                if (cell.getY() - lastCell.getY() == -1) {
                    /// GÓRA
                    if (lastChar == 'N') {
                        counter++;
                    }
                    else {
                        if (i != 1) {
                            directions.add(lastChar);
                            steps.add(counter);
                        }

                        lastChar = 'N';
                        counter = 0;
                    }
                }
                else if (cell.getX() - lastCell.getX() == 1) {
                    /// PRAWO
                    if (lastChar == 'E') {
                        counter++;
                    }
                    else {
                        if (i != 1) {
                            directions.add(lastChar);
                            steps.add(counter);
                        }

                        lastChar = 'E';
                        counter = 0;
                    }
                }
                else if (cell.getY() - lastCell.getY() == 1) {
                    /// DÓŁ
                    if (lastChar == 'S') {
                        counter++;
                    }
                    else {
                        if (i != 1) {
                            directions.add(lastChar);
                            steps.add(counter);
                        }

                        lastChar = 'S';
                        counter = 0;
                    }
                }
                else if (cell.getX() - lastCell.getX() == -1) {
                    /// LEWO
                    if (lastChar == 'W') {
                        counter++;
                    }
                    else {
                        if (i != 1) {
                            directions.add(lastChar);
                            steps.add(counter);
                        }

                        lastChar = 'W';
                        counter = 0;
                    }
                }
            }

            directions.add(lastChar);
            steps.add(counter);
        }
        else
            doWritePath = false;


    // Zapis do pliku
        try (DataOutputStream dos = new DataOutputStream(new FileOutputStream(filePath))) {
            /// SEKCJA 1
            dos.writeInt( Integer.reverseBytes(0x52524243));  // head
            dos.writeByte(0x1B);        // znak ESC
            dos.writeShort(Short.reverseBytes((short)(2*maze.getWidth() + 1)));
            dos.writeShort(Short.reverseBytes((short)(2*maze.getHeight() + 1)));
            dos.writeShort(Short.reverseBytes((short)(2*maze.getStartCell().getX() + 2)));
            dos.writeShort(Short.reverseBytes((short)(2*maze.getStartCell().getY() + 2)));
            dos.writeShort(Short.reverseBytes((short)(2*maze.getEndCell().getX() + 2)));
            dos.writeShort(Short.reverseBytes((short)(2*maze.getEndCell().getY() + 2)));
            for (int i = 0; i < 12; i++) { dos.writeByte(0); }  // reserved
            dos.writeInt(Integer.reverseBytes(symbols.size()));  // liczba słów kodowych
            if (doWritePath) {
                dos.writeInt(Integer.reverseBytes(320 + 24 * symbols.size()));    // offset do rozwiązania
            }
            else {
                dos.writeInt(0);
            }
            dos.writeByte('#');
            dos.writeByte(Settings.WALL_CHAR);
            dos.writeByte(Settings.PASS_CHAR);

            /// SEKCJA 2
            for (int i = 0; i < symbols.size(); i++) {
                dos.writeByte('#');
                dos.writeByte(symbols.get(i));
                dos.writeByte(countList.get(i));
            }

            if (doWritePath) {
                /// SEKCJA 3 (o ile występuje)
                dos.writeInt(Integer.reverseBytes(0x52524243));
                dos.writeInt(Integer.reverseBytes(stepsCounter - 1)); // w pliku omawiającym plik binarny jest błąd — 8 bitów to za mało na zapis ścieżki, więc daliśmy 32 bity

                // SEKCJA 4 (o ile występuje)
                for (int i = 0; i < directions.size(); i++) {
                    dos.writeByte(directions.get(i));
                    dos.writeByte(steps.get(i));
                }
            }
        }
        catch (IOException e) {
            JOptionPane.showMessageDialog(null, "Nie udało się zapisać pliku binarnego!" + e.getMessage(), "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }

        System.out.println("Pomyślnie zapisano plik do: " + filePath + ".");
    }





    private void readBinaryFile(String path) throws IOException, InvalidFileException {
        try (DataInputStream dis = new DataInputStream(new FileInputStream(path))) {
            head = Integer.reverseBytes(dis.readInt());
            escape = dis.readByte();
            width = Short.reverseBytes(dis.readShort());
            height = Short.reverseBytes(dis.readShort());
            entryX = Short.reverseBytes(dis.readShort());
            entryY = Short.reverseBytes(dis.readShort());
            exitX = Short.reverseBytes(dis.readShort());
            exitY = Short.reverseBytes(dis.readShort());
            for (int i = 0; i < 12; i++)    // reserved
                dis.readByte();
            counter = Integer.reverseBytes(dis.readInt());
            solutionOffset = Integer.reverseBytes(dis.readInt());
            separator = dis.readByte();
            word_wall = dis.readByte();
            word_path = dis.readByte();

            maze_cells = new char[this.width][this.height];


        // Zmiana ustawień
            Settings.PASS_CHAR = word_path;
            Settings.WALL_CHAR = word_wall;


        // czytanie słów kodowych
            int x = 0;
            int row_counter = 0;

            /// KOLEJNE ZNAKI KODOWE
            byte sep = 0;
            byte val = 0;
            int count = 0;

            for (int i = 0; i < counter; i++) {
            // Przeczytanie słowa kodowego
                sep = dis.readByte();
                if( sep != this.separator) {
                    throw new InvalidFileException("Wystąpił błędny znak");
                }

                val = dis.readByte();
                if (val != this.word_wall && val != this.word_path) {
                    throw new InvalidFileException("Wystąpił nieznany znak ściany lub przejścia");
                }

                count = dis.readUnsignedByte();


            // Warunek końcowy — jako zabezpieczenie
                if (row_counter == this.height) {
                    // przeczytano cały labirynt
                    break;
                }


            // Wypisywanie znaku w liniach
                for (int j = 0; j <= count; j++) {
                    if (x == this.width) {
                        // osiągnięto całą linię w labiryncie — przechodzę do kolejnej
                        x = 0;
                        ++row_counter;
                    }
                    this.maze_cells[x][row_counter] = (char) val;

                    ++x;
                }
            }

        } catch (IOException e) {
            throw new IOException(e.getMessage());
        }


    // Ustawienie wejścia/wyjścia
        maze_cells[this.entryX - 1][this.entryY - 1] = (char) Settings.ENTRY_CHAR;
        maze_cells[this.exitX - 1][this.exitY - 1] = (char) Settings.EXIT_CHAR;
    }

    @Override
    public int getWidth() {
        return (this.width - 1) / 2;
    }
    @Override
    public int getHeight() {
        return (this.height - 1) / 2;
    }

    @Override
    public int getStartCellX() {
        return (this.entryX - 2) / 2;
    }

    @Override
    public int getStartCellY() {
        return (this.entryY - 2) / 2;
    }

    @Override
    public int getEndCellX() {
        return (this.exitX - 2) / 2;
    }

    @Override
    public int getEndCellY() {
        return (this.exitY - 2) / 2;
    }

    @Override
    public Cell getCellAt(int x, int y) throws NoSuchCellException{
        int mapX = 2*x + 1;
        int mapY = 2*y + 1;

        // sprawdzenie istnienia komórki
        if (maze_cells[mapX][mapY] == 0) {
            throw new NoSuchCellException();
        }

        Cell cell = new Cell(x, y);

        //UP
        if(maze_cells[mapX][mapY - 1] == this.word_path ){
            cell.setPass(true, Direction.Up);
        }

        if(maze_cells[mapX][mapY + 1] == this.word_path ){
            cell.setPass(true, Direction.Down);
        }

        if(maze_cells[mapX + 1][mapY] == this.word_path ){
            cell.setPass(true, Direction.Right);
        }

        if(maze_cells[mapX - 1][mapY] == this.word_path ){
            cell.setPass(true, Direction.Left);
        }

        return cell;
    }
}
