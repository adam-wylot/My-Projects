package GUI;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

import GUI.panels.*;
import common.Core;
import common.Maze;
import common.Settings;
import numerators.PanelType;

public class MainWindow extends JFrame {
// Zmienne
    private PanelType panelType;

    private MenuBar menuBar;
    private ToolsPanel toolsPanel;
    private JPanel activePanel;

    private MazePanel mazePanel;


// Konstruktory
    public MainWindow() {
        this(1600, 900);
    }

    public MainWindow(int width, int height) {
        super();

    // Ustawianie okna
        setSize( (int)(Core.ScaleX() * width), (int)(Core.ScaleY() * height));
        setMinimumSize(new Dimension((int)(Core.ScaleX() * 1200), (int)(Core.ScaleY() * 400)));

        setTitle(Core.ProgramName());
        setLayout(new BorderLayout());
        setBackground(Settings.getBackgroundColor());

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        loadIcon("/icon.png");


    // Inicjalizacja obiektów
        menuBar = new MenuBar(this);
        this.setJMenuBar(menuBar);

        // panel z narzędziami
        toolsPanel = new ToolsPanel( (int)(Core.ScaleX() * width) / 5, (int)(Core.ScaleY() * height));
        this.add(toolsPanel, BorderLayout.WEST);

        // panel z labiryntem
        mazePanel = new MazePanel();

        // panel aktywny
        activePanel = mazePanel;  // domyślny panel
        panelType = PanelType.Maze;
        this.add(activePanel, BorderLayout.CENTER);


        setVisible(true);   // Wyświetlenie okna — włączenie okna
    }


    // Gettery
    public MazePanel getMazePanel() { return mazePanel; }
    public ToolsPanel getToolsPanel() { return toolsPanel; }
    public MenuBar getMenu() { return menuBar; }


    // Metody
    public void setActivePanel(PanelType panelType) {
        this.panelType = panelType;
        this.remove(activePanel);

        switch(panelType) {
            case Maze: {
                activePanel = mazePanel;
                if (mazePanel.isMazeLoaded()) {
                    toolsPanel.getFindPathButton().setVisible(true);
                    toolsPanel.getStartButton().setVisible(true);
                    toolsPanel.getEndButton().setVisible(true);
                    menuBar.getPathMenu().setEnabled(true);
                }
                break;
            }

            case Settings: {
                activePanel = new SettingsPanel();
                toolsPanel.getFindPathButton().setVisible(false);
                toolsPanel.getStartButton().setVisible(false);
                toolsPanel.getEndButton().setVisible(false);
                break;
            }
        }

        this.add(activePanel, BorderLayout.CENTER);
        this.revalidate();
        this.repaint();
    }

    public void loadMaze(Maze maze) {
        mazePanel.renderMaze(maze);

        // odświeżenie okna
        if (panelType == PanelType.Maze) {
            this.remove(activePanel);
            activePanel = mazePanel;
            this.add(activePanel, BorderLayout.CENTER);

            toolsPanel.getFindPathButton().setVisible(true);
            toolsPanel.getStartButton().setVisible(true);
            toolsPanel.getEndButton().setVisible(true);
            menuBar.getPathMenu().setEnabled(false);
        }
        this.revalidate();
    }

    private void loadIcon(String path) {
        try {
            ImageIcon icon = new ImageIcon(ImageIO.read(Objects.requireNonNull(getClass().getResource(path))));
            setIconImage(icon.getImage());
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony programu");
        }
    }
}
