import GUI.MainWindow;
import common.Core;
import common.WordList;
import tests.MainTest;

public class Main {
    public static void main(String[] args) {
        MainTest.run();

    // Inicjalizacja okna głównego programu
        Core.start();
        MainWindow mainWindow = Core.Instance().getMainWindow();
    }
}