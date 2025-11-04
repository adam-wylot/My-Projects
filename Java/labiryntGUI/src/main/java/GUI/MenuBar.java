package GUI;

import common.Core;
import common.fileHandlers.BinaryFH;
import common.fileHandlers.TextFH;

import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.File;


public class MenuBar extends JMenuBar {
// Zmienne
    private JMenu mazeMenu;
    private JMenu pathMenu;
    private JFrame mainWindow;


    static {
        // Zmiana nazewnictwa w oknie
        UIManager.put("FileChooser.saveButtonText", "Zapisz");
        UIManager.put("FileChooser.saveButtonToolTipText", "Zapisz wybrany plik");
        UIManager.put("FileChooser.cancelButtonText", "Anuluj");
        UIManager.put("FileChooser.openButtonText", "Otwórz");
        UIManager.put("FileChooser.saveDialogTitleText", "Zapisz");
        UIManager.put("FileChooser.lookInLabelText", "Szukaj w:");
        UIManager.put("FileChooser.fileNameLabelText", "Nazwa pliku:");
        UIManager.put("FileChooser.filesOfTypeLabelText", "Typ pliku:");
        UIManager.put("FileChooser.upFolderToolTipText", "W górę");
        UIManager.put("FileChooser.homeFolderToolTipText", "Pulpit");
        UIManager.put("FileChooser.newFolderToolTipText", "Utwórz nowy folder");
        UIManager.put("FileChooser.newFolderAccessibleName", "Nowy folder");
        UIManager.put("FileChooser.newFolderButtonText", "Nowy folder");
        UIManager.put("FileChooser.listViewButtonToolTipText", "Lista");
        UIManager.put("FileChooser.detailsViewButtonToolTipText", "Szczegóły");
        UIManager.put("FileChooser.fileNameHeaderText", "Nazwa pliku:");
        UIManager.put("FileChooser.fileNameLabelText", "Nazwa pliku:");
        UIManager.put("FileChooser.fileSizeHeaderText", "Rozmiar:");
        UIManager.put("FileChooser.fileTypeHeaderText", "Typ:");
        UIManager.put("FileChooser.fileDateHeaderText", "Data modyfikacji");
        UIManager.put("FileChooser.fileAttrHeaderText", "Atrybuty");
        UIManager.put("FileChooser.saveInLabelText", "Zapisz w:");
        UIManager.put("FileChooser.folderNameLabelText", "Nazwa pliku / Ścieżka:");
        UIManager.put("FileChooser.saveAsDialogTitleText", "Zapisz jako:");
        UIManager.put("FileChooser.homeFolderToolTipText", "Dokumenty");
        UIManager.put("FileChooser.desktopName", "Pulpit");
        UIManager.put("FileChooser.listViewButtonToolTipText", "Lista");
        UIManager.put("FileChooser.saveDialogFileNameLabelText", "Nazwa pliku:");
        UIManager.put("FileChooser.saveDialogFileNameLabelText", "Nazwa pliku:");
        UIManager.put("FileChooser.byDateText", "Nazwa pliku");
        UIManager.put("FileChooser.byNameText", "Data Modyfikacji");
    }


// Konstruktor
    public MenuBar(JFrame frame) {
        super();

        mainWindow = frame;

    // Maze Menu
        mazeMenu = new JMenu("Labirynt");

        // Opcje
        JMenuItem openMM = createOpenMM();
        JMenuItem saveMM = createSaveMM(false);
        JMenuItem saveAsMM = createSaveAsMM(false);

        mazeMenu.add(openMM);
        mazeMenu.addSeparator();
        mazeMenu.add(saveMM);
        mazeMenu.add(saveAsMM);

    // Path Menu
        pathMenu = new JMenu("Ścieżka");
        //JMenuItem findPathPM = createFindPathPM();
        JMenuItem savePM = createSaveMM(true);
        JMenuItem saveAsPM = createSaveAsMM(true);

        //pathMenu.add(findPathPM);
        //pathMenu.addSeparator();
        pathMenu.add(savePM);
        pathMenu.add(saveAsPM);

        this.add(mazeMenu);
        this.add(pathMenu);

        pathMenu.setEnabled(false);
    }

    public JMenu getPathMenu() { return pathMenu; }

    private JMenuItem createOpenMM() {
        JMenuItem item = new JMenuItem("Otwórz...");
        item.addActionListener(e -> {
            // Utworzenie obiektu
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

            /// Dodanie filtrów
            FileFilter allFilesFilter = new FileFilter() {
                @Override
                public boolean accept(File file) {
                    return true;
                }

                @Override
                public String getDescription() {
                    return "Wszystkie pliki (.*)";
                }
            };
            fileChooser.addChoosableFileFilter(allFilesFilter);
            fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("Pliki tekstowe (.txt)", "txt"));
            fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("Pliki binarne (.bin)", "bin"));

            fileChooser.setFileFilter(allFilesFilter);
            fileChooser.setAcceptAllFileFilterUsed(false);

            // Zmiana nazw przycisku i okna
            fileChooser.setDialogTitle("Wybierz plik");
            fileChooser.setApproveButtonText("Otwórz");


            int result = fileChooser.showOpenDialog(mainWindow);
            if (result == JFileChooser.APPROVE_OPTION) {

                // Pobranie ścieżki z wybranego pliku (aby wczytać labirynt)
                File selectedFile = fileChooser.getSelectedFile();
                Core.loadMazeFile(selectedFile.getAbsolutePath());
            }
        });
        return item;
    }

    public JMenuItem createSaveMM(boolean doWritePath) {
        JMenuItem item = new JMenuItem("Zapisz");
        item.addActionListener(e -> {
            if (Core.getMaze() == null) {
                JOptionPane.showMessageDialog(null, "Nie wczytano żadnego labiryntu!", "Błąd", JOptionPane.ERROR_MESSAGE);
                return;
            }

            TextFH.saveMaze(Core.getMaze(), Core.getMaze().getMazePath(), doWritePath);
        });

        return item;
    }

    public JMenuItem createSaveAsMM(boolean doWritePath) {
        JMenuItem item = new JMenuItem("Zapisz jako...");
        item.addActionListener(e -> {
            if (Core.getMaze() == null) {
                JOptionPane.showMessageDialog(null, "Nie wczytano żadnego labiryntu!", "Błąd", JOptionPane.ERROR_MESSAGE);
                return;
            }

            // Utworzenie obiektu
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setFileSelectionMode(JFileChooser.SAVE_DIALOG);

            /// Dodanie filtrów
            fileChooser.setAcceptAllFileFilterUsed(false);
            fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("Plik tekstowy (.txt)", "txt"));
            fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("Plik binarny (.bin)", "bin"));
            fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("Plik graficzny (.png)", "png"));


            // Zmiana nazw przycisku i okna
            fileChooser.setDialogTitle("Zapisz plik");


            int result = fileChooser.showSaveDialog(mainWindow);
            if (result == JFileChooser.APPROVE_OPTION) {
                // Uzyskanie wybranego filtru
                FileNameExtensionFilter selectedFilter = (FileNameExtensionFilter) fileChooser.getFileFilter();
                String extension = selectedFilter.getExtensions()[0];

                // Sprawdzenie, czy nazwa pliku ma odpowiednie rozszerzenie
                String filePath = fileChooser.getSelectedFile().getAbsolutePath();
                if (!filePath.toLowerCase().endsWith("." + extension.toLowerCase())) {
                    filePath += "." + extension.toLowerCase();
                }

                // Zapisanie labiryntu
                switch (extension.toLowerCase()) {
                    case "txt": {
                        TextFH.saveMaze(Core.getMaze(), filePath, doWritePath);
                        break;
                    }

                    case "bin": {
                        BinaryFH.saveMaze(Core.getMaze(), filePath, doWritePath);
                        break;
                    }

                    case "png": {
                        Core.getMainWindow().getMazePanel().getCanvasPanel().saveMazeImage(filePath, doWritePath);
                        break;
                    }
                }

            }
        });


        return item;
    }




    // -------------------------------------------------------------------------------




/*    private JMenuItem createFindPathPM() {
        JMenuItem menuItem = new JMenuItem("Znajdź");
        menuItem.addActionListener(e -> {
            Maze maze = Core.getMaze();
            if (maze == null)
                return;


            PathFinder pathFinder = new PathFinder(maze);
            ICallBack callback = () -> {
                if (pathFinder.FoundPath()) {
                    Core.getMainWindow().getMazePanel().markPath(pathFinder.getPath());
                }
            };

            pathFinder.setCallback(callback);

            pathFinder.start();
        });

        return menuItem;
    }*/
}
