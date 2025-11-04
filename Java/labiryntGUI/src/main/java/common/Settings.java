package common;

import javax.swing.*;
import java.awt.*;

public class Settings {
// Zmienne
    public static byte WALL_CHAR = 'X';
    public static byte PASS_CHAR = ' ';
    public static byte ENTRY_CHAR = 'P';
    public static byte EXIT_CHAR = 'K';
    public static byte PATH_CHAR = '*';


    public static Color PATH_COLOR = new Color(0, 255, 120);
    private static Color BACKGROUND_COLOR = new Color(186, 241, 216);


// Konstruktor
    private Settings() {}


// Gettery
    public static Color getBackgroundColor() { return BACKGROUND_COLOR; }


// Settery
    public static void setBackgroundColor(Color color) {
        BACKGROUND_COLOR = color;
        Core.getMainWindow().getMazePanel().getCanvasPanel().revalidate();
        SwingUtilities.invokeLater(Core.getMainWindow().getMazePanel().getCanvasPanel()::repaint);
    }
}
