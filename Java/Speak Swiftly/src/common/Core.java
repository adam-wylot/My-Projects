package common;

import GUI.MainWindow;

import java.awt.*;
import java.io.File;
import java.io.IOException;

public class Core {
// Instancja
    private static Core instance;

// Zmienne
    public static final String programName = "Speak Swiftly";
    public static final char systemSep = System.getProperty("os.name").toLowerCase().contains("win") ? '\\' : '/';
    public static final String wordListsPath = "assets" + systemSep + "words";
    public static final String masterWLname = "Master";


    private MainWindow mainWindow;
    private Flashcards flashcards;

    private int screenWidth;
    private int screenHeight;
    private double horScale;
    private double verScale;

    private Font programFont;
    private Color programColor;


// Konstruktor (prywatny)
    private Core() {
        // szerokość ekranu
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Dimension screenSize = toolkit.getScreenSize();

        screenWidth = (int) screenSize.getWidth();
        screenHeight = (int) screenSize.getHeight();

        // wyznaczanie skali dla obiektów
        horScale = (double) screenWidth / 2560;
        verScale = (double) screenHeight / 1440;

        flashcards = new Flashcards();

        // Własna czcionka
        try {
            programFont = Font.createFont(Font.TRUETYPE_FONT, new File("assets\\fonts\\WorkSans.ttf"));
            programFont = programFont.deriveFont(20f);
        }
        catch (IOException | FontFormatException e) {
            programFont = new Font("Arial", Font.PLAIN, 20);
        }



        programColor = Color.darkGray;
    }

// Gettery
    public static Core Instance() {
        if( instance == null ) {
            // zabezpieczenie
            instance = new Core();
        }

        return instance;
    }

    public MainWindow getMainWindow() {
        if (mainWindow == null) {
            mainWindow = new MainWindow();
        }

        return mainWindow;
    }

    public Flashcards getFlashcards() { return flashcards; }
    public double getHorScale() { return this.horScale; }
    public double getVerScale() { return this.verScale; }
    public Font getProgramFont() { return this.programFont; }
    public Color getProgramColor() { return this.programColor; }


// Metody
    public static void start() {
        System.out.println("=== Log: [" + programName + "] ===");
        instance = new Core();

        // Wczytanie danych z plików
        WordList.loadWordLists(wordListsPath);
    }

    public static void end() {
        WordList.saveWordLists(Core.wordListsPath);  // zapisanie zestawów słów
        printLog("kończę działanie");
        System.out.println("============================");
    }

    public static void printErr(String message) {
        System.err.println(programName + ": błąd: " + message + "!");
    }

    public static void printLog(String message) {
        System.out.println(programName + ": " + message + ".");
    }
}
