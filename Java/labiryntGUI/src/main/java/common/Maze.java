package common;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;

import common.fileHandlers.InputFH;
import exceptions.*;

public class Maze {
// Zmienne
    private final int width;
    private final int height;
    private final Cell[][] cells;     /// [x][y]
    private Cell startCell;
    private Cell endCell;
    private ArrayList<Cell> path;

    private InputFH inputFH;


// Konstruktor
    public Maze(String filePath) throws MazeNotLoadedException {
    // Czytanie pliku z labiryntem
        path = null;

        try {
            inputFH = new InputFH(filePath);
        }
        catch (FileNotFoundException e) {
            throw new MazeNotLoadedException("nie odnaleziono pliku z labiryntem (" + filePath + ")." + e.getMessage() );
        }
        catch (IOException e) {
            throw new MazeNotLoadedException("nie udało sie odczytać pliku (" + filePath + ")." + e.getMessage() );
        }
        catch (UnknownFileTypeException e) {
            throw new MazeNotLoadedException("podano plik o nieznanym formacie." + e.getMessage());
        }
        catch (InvalidFileException e) {
            throw new MazeNotLoadedException("labirynt ma nieprawidłową budowę.\n\n" + e.getMessage());
        }


    // Ustawianie zmiennych obiektu
        // rozmiary labiryntu
        width = inputFH.FileHandler().getWidth();
        height = inputFH.FileHandler().getHeight();

        try {
            cells = inputFH.readCells(width, height);
        }
        catch (NoSuchCellException e) {
            throw new MazeNotLoadedException("nie udało się wczytać komórki — komórka nie istnieje");
        }


        // wczytanie komórki startowej i końcowej
        try {
            startCell = cells[ inputFH.FileHandler().getStartCellX() ][ inputFH.FileHandler().getStartCellY() ];
            endCell = cells[ inputFH.FileHandler().getEndCellX() ][ inputFH.FileHandler().getEndCellY() ];
        }
        catch (NoSuchCellException e) {
            throw new MazeNotLoadedException("nie udało się wczytać komórki wejściowej lub wyjściowej w labiryncie");
        }
    }


// Gettery
    public ArrayList<Cell> getPath() { return path; }
    public Cell getStartCell() { return startCell; }
    public Cell getEndCell() { return endCell; }
    public int getWidth() { return width; }
    public int getHeight() { return height; }
    public Cell getCellAt(int x, int y) { return cells[x][y]; }
    public String getMazePath() { return inputFH.getPath(); }


// Settery
    public void setPath(ArrayList<Cell> list) { path = list; }


// Metody
    public void mazeClickedAt(int x, int y) {
        if (Core.getMainWindow().getToolsPanel().getStartButton().isActive()) {
            startCell = cells[x][y];
            System.out.println("Ustawiono nowe wejście na komórkę: (" + x + ", " + y +")");

            Core.getMainWindow().getToolsPanel().getStartButton().setActive(false);
        }
        else if (Core.getMainWindow().getToolsPanel().getEndButton().isActive()) {
            endCell = cells[x][y];
            System.out.println("Ustawiono nowe wyjście na komórkę: (" + x + ", " + y +")");

            Core.getMainWindow().getToolsPanel().getEndButton().setActive(false);
        }
    }


}
