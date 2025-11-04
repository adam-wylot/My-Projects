package GUI.buttons;

import common.Core;
import common.Maze;
import common.PathFinder;
import interfaces.ICallBack;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;
import java.util.concurrent.ExecutionException;

public class FindPathButton extends JButton {
// Zmienne
    private ImageIcon icon;

// Konstruktor
    public FindPathButton(Dimension dimension) {
        super();
        Image image;

        try {
            image = ImageIO.read(Objects.requireNonNull(getClass().getResource("/images/search.png")));
            image = image.getScaledInstance((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 75), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 75), Image.SCALE_SMOOTH);
            icon = new ImageIcon(image);
            setIcon(icon);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony przycisku \"Znajdź ścieżkę\"");
        }


        setText("Znajdź ścieżkę");
        setPreferredSize(dimension);

        setBackground(new Color(6, 170, 247));
        setFocusPainted(false);
        setBorder(BorderFactory.createEtchedBorder());

        setHorizontalAlignment(SwingConstants.LEFT);

        // funkcjonalność przycisku
        addActionListener(e -> click());
        setVisible(false);
    }


// Metody
    private void click() {
        Maze maze = Core.getMaze();
        if (maze == null)
            return;


        PathFinder pathFinder = new PathFinder(maze);
        ICallBack callback = () -> {
            if (pathFinder.FoundPath()) {
                Core.getMainWindow().getMazePanel().markPath(pathFinder.getPath());
                Core.getMainWindow().getMenu().getPathMenu().setEnabled(true);
            }
            else {
                JOptionPane.showMessageDialog(null, "Nie znaleziono żadnej ścieżki w labiryncie.", Core.ProgramName(), JOptionPane.INFORMATION_MESSAGE);
            }
        };

        pathFinder.setCallback(callback);

        pathFinder.start();
    }
}
