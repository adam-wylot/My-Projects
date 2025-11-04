package GUI.panels;

import common.Cell;
import common.Core;
import common.Maze;
import common.Settings;
import interfaces.ICallBack;
import numerators.Direction;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Objects;

public class CanvasPanel extends JLayeredPane {
    JPanel background;
    JPanel mazeP;
    JPanel overlayP;

// Zmienne
    private Maze maze;
    private ArrayList<Cell> path;
    private BufferedImage mazeImage;
    private BufferedImage finalImage;
    private BufferedImage chosenImage;
    private boolean isRenderingThreadActive;

    private int wallThickness;
    private int cellSize;
    private double renderScale;

    private Color wallColor;
    private Color backgroundColor;

    private boolean mouseInArea;
    private int mouseX;
    private int mouseY;


// Konstruktor
    public CanvasPanel() {
        super();

        background = new JPanel();
        background.setBackground(Settings.getBackgroundColor());
        background.setBounds(0, 0, 1000000, 1000000);   /// duże liczby, bo z Integer.MAX_VALUE coś nie działało
        mazeP = initMazePanel();
        overlayP = initOverlayPanel();
        overlayP.setOpaque(false);

        this.add(background, JLayeredPane.DEFAULT_LAYER);
        this.add(mazeP, JLayeredPane.PALETTE_LAYER);
        this.add(overlayP, JLayeredPane.POPUP_LAYER);

        maze = null;
        path = null;
        isRenderingThreadActive = false;
        mazeImage = null;
        finalImage = null;
        try {
            chosenImage = ImageIO.read(Objects.requireNonNull(getClass().getResource("/images/chosen.png")));
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony komórki wskazywanej przez myszkę w labiryncie");
            chosenImage = null;
        }

        wallThickness = 2;
        cellSize = 10;
        renderScale = 1.;

        wallColor = Color.BLACK;
        backgroundColor = Color.WHITE;

        mouseInArea = false;
        mouseX = 0;
        mouseY = 0;
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
    }




// Settery
    public void setPath(ArrayList<Cell> path) { this.path = path; }
    public void setScale(double value) {
        if (finalImage == null)
            return;

        if (value <= 0 || value > 5.0)
            return;

        renderScale = value;
        setCanvasSize((int)(finalImage.getWidth() * renderScale), (int)(finalImage.getHeight() * renderScale));
        SwingUtilities.invokeLater(mazeP::repaint);
    }




// Gettery
    public double getScale() { return renderScale; }



// Metody
    public void saveMazeImage(String path, boolean withPath) {
        try {
            File outputfile = new File(path);

            if (withPath) {
                ImageIO.write(finalImage, "png", outputfile);
            }
            else {
                ImageIO.write(mazeImage, "png", outputfile);
            }
        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, "Nie udało się zapisać obrazu!", "Błąd", JOptionPane.ERROR_MESSAGE);
        }

        JOptionPane.showMessageDialog(null, "Pomyślnie zapisano obraz.", Core.ProgramName(), JOptionPane.INFORMATION_MESSAGE);
    }

    public void renderMaze(Maze maze) {
        // inny wątek jest już aktywny
        if (isRenderingThreadActive)
            return;

        // wczytanie nowego labiryntu (innego niż poprzednio wczytany)
        if (this.maze != maze) {
            this.maze = maze;
            path = null;
        }

        new Thread(() -> {
            isRenderingThreadActive = true;

            boolean flag = false;
            if (mazeImage != null) {
                mazeImage.flush();
                mazeImage = null;
                flag = true;
            }

            if (finalImage != null) {
                finalImage.flush();
                finalImage = null;
                flag = true;
            }

            if (flag)
                System.gc();

            int width = maze.getWidth() * cellSize;
            int height = maze.getHeight() * cellSize;
            BufferedImage tmpImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

            // Rysowanie labiryntu w pamięci (off-screen)
            Graphics2D g2d = tmpImage.createGraphics();
            drawMaze(g2d, width, height);
            g2d.dispose();  // zwolnienie zasobów zajmowanych przez g2d

            // Po zakończeniu rysowania odświeżenie panelu (EDT)
            setCanvasSize((int)(renderScale * width), (int)(renderScale * height));
            mazeImage = tmpImage;
            finalImage = tmpImage;

            SwingUtilities.invokeLater(mazeP::repaint);
            isRenderingThreadActive = false;
        }).start();
    }

    public void renderPath(ICallBack callBack) {
        if (isRenderingThreadActive || path == null)
            return;

        new Thread(() -> {
            isRenderingThreadActive = true;

            if (finalImage != null) {
                finalImage.flush();
                finalImage = null;
            }


            BufferedImage tmpImage = new BufferedImage(mazeImage.getWidth(), mazeImage.getHeight(), mazeImage.getType());

            // Rysowanie labiryntu w pamięci (off-screen)
            Graphics2D g2d = tmpImage.createGraphics();
            g2d.drawImage(mazeImage, 0, 0, null);   // narysowanie labiryntu jako tła

            g2d.setColor(Settings.PATH_COLOR);
            for (Cell cell : path) {
                paintCell(g2d, cell);
            }

            g2d.dispose();  // zwolnienie zasobów zajmowanych przez g2d

            // Po zakończeniu rysowania odświeżenie panelu (EDT)
            finalImage = tmpImage;

            if (callBack != null) {
                callBack.onThreadComplete();
            }

            SwingUtilities.invokeLater(mazeP::repaint);
            isRenderingThreadActive = false;
        }).start();
    }

    private void drawMaze(Graphics2D g2d, int width, int height) {
        // rysowanie planszy
        g2d.setColor(wallColor);
        g2d.fillRect(0, 0, width, height);

        // rysowanie komórek
        g2d.setColor(backgroundColor);

        for (int y=0; y < maze.getHeight(); y++) {
            for (int x=0; x < maze.getWidth(); x++) {
                paintCell(g2d, maze.getCellAt(x, y));
            }
        }
    }

    private void paintCell(Graphics2D g2d, Cell cell) {
        // Komórka
        g2d.fillRect(cell.getX() * cellSize + wallThickness, cell.getY() * cellSize + wallThickness, cellSize - 2 * wallThickness, cellSize - 2 * wallThickness);

        // Przejścia
        /// GÓRA
        if (cell.isWallOpen(Direction.Up)) {
            g2d.fillRect(cell.getX() * cellSize + wallThickness, cell.getY() * cellSize, cellSize - 2 * wallThickness, wallThickness);
        }

        /// PRAWO
        if (cell.isWallOpen(Direction.Right)) {
            g2d.fillRect(cell.getX() * cellSize + cellSize -wallThickness, cell.getY() * cellSize + wallThickness, wallThickness, cellSize - 2 * wallThickness);
        }

        /// DÓŁ
        if (cell.isWallOpen(Direction.Down)) {
            g2d.fillRect(cell.getX() * cellSize + wallThickness, cell.getY() * cellSize + cellSize -wallThickness, cellSize - 2 * wallThickness, wallThickness);
        }

        /// LEWO
        if (cell.isWallOpen(Direction.Left)) {
            g2d.fillRect(cell.getX() * cellSize, cell.getY() * cellSize + wallThickness, wallThickness, cellSize - 2 * wallThickness);
        }
    }

    private void setCanvasSize(int width, int height) {
        Dimension dim = new Dimension(width, height);
        JScrollPane scrollPane = null;

    // Znalezienie pasków do przewijania
        Container parent = getParent();
        while (parent != null && !(parent instanceof JScrollPane)) {
            // szukanie obiektu JScrollPane
            parent = parent.getParent();
        }

    // liczenie procentu przewiniętych pasków
        double vertPerc = 0.;
        double horPerc = 0.;

        if (parent != null) {
            scrollPane = (JScrollPane) parent;

            vertPerc = (scrollPane.getViewport().getViewRect().y + scrollPane.getViewport().getViewRect().height / 2.0) / scrollPane.getViewport().getViewSize().height;
            horPerc = (scrollPane.getViewport().getViewRect().x + scrollPane.getViewport().getViewRect().width / 2.0) / scrollPane.getViewport().getViewSize().width;
        }

    // Zmiana rozmiarów ekranów
        setPreferredSize(dim);
        this.setPreferredSize(dim);
        mazeP.setPreferredSize(dim);
        mazeP.setBounds(0, 0, width, height);
        overlayP.setPreferredSize(dim);
        overlayP.setBounds(0, 0, width, height);

        revalidate();

    // Zmiana rozmiarów i przerysowanie pasków do przewijania (o ile znaleziono)
        if (scrollPane != null) {   // musi być JScrollPane
            // zmiana lokalizacja — żeby nastąpiło przybliżenie/oddalenie w tym samym miejscu
            scrollPane.getVerticalScrollBar().setValue((int)((vertPerc * scrollPane.getViewport().getViewSize().height) - (scrollPane.getViewport().getViewRect().height / 2.0)));
            scrollPane.getHorizontalScrollBar().setValue((int)((horPerc * scrollPane.getViewport().getViewSize().width) - (scrollPane.getViewport().getViewRect().width / 2.0)));

            scrollPane.revalidate();    // ponowne sprawdzenie elementów
            scrollPane.repaint();   // przerysowanie
        }
    }

    public void changeBackgroundColor(Color color) {
        this.remove(background);

        background.setBackground(color);
        background.setBounds(0, 0, 1000000, 1000000);   /// duże liczby, bo z Integer.MAX_VALUE coś nie działało

        this.add(background, JLayeredPane.DEFAULT_LAYER);
        revalidate();
        repaint();
    }

    private void setMousePositionText(int x, int y) {
        mouseX = (int)(x / (cellSize * renderScale));
        mouseY = (int)(y / (cellSize * renderScale));

        if (mouseX >= maze.getWidth() || mouseY >= maze.getHeight() || mouseX < 0 || mouseY < 0) {
            Core.getMainWindow().getMazePanel().getScrollPanel().getUtilityPanel().setXYlabel(0, 0);
            return;
        }


        Core.getMainWindow().getMazePanel().getScrollPanel().getUtilityPanel().setXYlabel(mouseX+1, mouseY+1);
        SwingUtilities.invokeLater(overlayP::repaint);
    }

    private JPanel initMazePanel() {
        JPanel panel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);

                if (finalImage != null) {
                    g.drawImage(finalImage, 0, 0, (int)(finalImage.getWidth() * renderScale), (int)(finalImage.getHeight() * renderScale), null);
                }

                g.dispose();
            }
        };

        panel.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (maze != null) {
                    maze.mazeClickedAt((int)(e.getX() / (renderScale * cellSize)), (int)(e.getY() / (renderScale * cellSize)));
                }
            }
        });

        panel.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {}
            @Override
            public void mousePressed(MouseEvent e) {}
            @Override
            public void mouseReleased(MouseEvent e) {}

            @Override
            public void mouseEntered(MouseEvent e) {
                mouseInArea = true;
            }

            @Override
            public void mouseExited(MouseEvent e) {
                mouseInArea = false;
                mouseX = 0;
                mouseY = 0;
                SwingUtilities.invokeLater(overlayP::repaint);
                Core.getMainWindow().getMazePanel().getScrollPanel().getUtilityPanel().setXYlabel(0, 0);
            }
        });

        panel.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseMoved(MouseEvent e) {
                setMousePositionText(e.getX(), e.getY());
            }

            @Override
            public void mouseDragged(MouseEvent e) {
                setMousePositionText(e.getX(), e.getY());
            }
        });

        return panel;
    }

    private JPanel initOverlayPanel() {
        JPanel panel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);

                if (chosenImage != null && mouseInArea) {
                    g.drawImage(chosenImage, (int)(mouseX * cellSize * renderScale), (int)(mouseY * cellSize * renderScale), (int)(renderScale * 10), (int)(renderScale * 10), null);
                }

                g.dispose();
            }

            @Override
            public boolean isOpaque() {
                return false;
            }
        };

        return panel;
    }
}
