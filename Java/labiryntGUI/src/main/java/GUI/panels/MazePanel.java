package GUI.panels;

import common.Cell;
import common.Maze;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class MazePanel extends JPanel {
// Zmienne
    private boolean mazeLoaded;
    private CanvasPanel canvasPanel;
    private ScrollPanel scrollPanel;


// Konstruktory
    public MazePanel() {
        super();

        setLayout(new BorderLayout());
        mazeLoaded = false;

        canvasPanel = new CanvasPanel();

        scrollPanel = new ScrollPanel(canvasPanel);
        this.add(scrollPanel, BorderLayout.CENTER);
    }


// Gettery
    public ScrollPanel getScrollPanel() { return scrollPanel; }
    public CanvasPanel getCanvasPanel() { return canvasPanel; }
    public boolean isMazeLoaded() { return mazeLoaded; }


// Metody
    public void markPath(ArrayList<Cell> path) {
        canvasPanel.setPath(path);
        canvasPanel.renderPath(null);
    }

    public void renderMaze(Maze maze) {
        canvasPanel.renderMaze(maze);
        mazeLoaded = true;
    }

    public void zoom(double value) {
        canvasPanel.setScale(canvasPanel.getScale() + value);
        scrollPanel.revalidate();
    }
}
