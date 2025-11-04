package common.fileHandlers;

import common.Cell;
import exceptions.*;
import interfaces.IFileHandler;
import numerators.FileType;

import java.io.*;


public class InputFH {
    // Zmienne
    private final FileType fileType;
    private final String filePath;
    private final IFileHandler fileHandler;


    // Konstruktor
    public InputFH(String path) throws FileNotFoundException, IOException, UnknownFileTypeException, InvalidFileException {
        // Sprawdzenie, czy plik istnieje
        File file = new File(path);

        if (!file.exists()) {
            throw new FileNotFoundException();
        }


        // Ustawienie zmiennych obiektu
        filePath = path;
        fileType = recognizeFileType(path);
        switch (fileType) {
            case binary: {
                fileHandler = new BinaryFH(path);
                break;
            }

            case text: {
                fileHandler = new TextFH(path);
                break;
            }

            default:
                throw new UnknownFileTypeException();
        }
    }


    // Gettery
    public IFileHandler FileHandler() { return fileHandler; }
    public String getPath() { return filePath; }


    // Metody
    private FileType recognizeFileType(String path) throws IOException, UnknownFileTypeException {
        if (BinaryFH.isBinaryFile(path)) {
            return FileType.binary;
        }
        if (TextFH.isTextFile(path)) {
            return FileType.text;
        }

        throw new UnknownFileTypeException();
    }

    public Cell[][] readCells(int width, int height) throws NoSuchCellException {
        Cell[][] cells = new Cell[width][height];

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cells[x][y] = fileHandler.getCellAt(x, y);
            }
        }

        return cells;
    }
}