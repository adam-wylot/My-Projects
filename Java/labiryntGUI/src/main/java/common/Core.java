package common;

import GUI.MainWindow;
import exceptions.MazeNotLoadedException;
import interfaces.ICallBack;

import javax.swing.*;
import java.awt.*;

public class Core {
// Zmienne
    private static final String programName;
    private static final double scaleX;
    private static final double scaleY;

    private static MainWindow mainWindow;
    private static Maze maze;


// Blok inicjalizacyjny zmiennych statycznych
    static {
        programName = "MAZE Solver";

        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        scaleX = (double) screenSize.width / 2560.0;
        scaleY = (double) screenSize.height / 1440.0;

        mainWindow = null;
        maze = null;
    }

// Zablokowanie konstruktora
    private Core() {}


// Gettery
    public static String ProgramName() { return programName; }
    public static double ScaleX() { return scaleX; }
    public static double ScaleY() { return scaleY; }
    public static MainWindow getMainWindow() { return mainWindow; }
    public static Maze getMaze() { return maze; }


// Settery


// Metody
    public static void start() {
        mainWindow = new MainWindow();
    }

    public static void printError(String message) { System.err.println(programName + ": błąd: " + message + "!"); }

    public static void loadMazeFile(String path) {
        // Stworzenie obiektu callback w wątku głównym, żeby po wykonaniu się zadania w wątku wczytującym labirynt rozpoczęło się wczytywanie labiryntu do panelu
        ICallBack callback = () -> mainWindow.loadMaze(maze);

        // Wczytywanie pliku z labiryntem do programu na oddzielnym wątku (żeby zapobiec zacięciu się programu przy dużych plikach)
        new Thread(() -> {
            try {
                maze = new Maze(path);
                callback.onThreadComplete();
            }
            catch (MazeNotLoadedException e) {
                JOptionPane.showMessageDialog(null,"Nie udało się wczytać labiryntu:\n" + e.getMessage() + "!", "Błąd", JOptionPane.ERROR_MESSAGE);
            }
        }).start();
    }
}
