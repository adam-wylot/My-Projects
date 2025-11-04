package GUI.buttons;

import common.Core;
import numerators.PanelType;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

public class MazeButton extends JButton {
// Konstruktor
    public MazeButton(Dimension dimension) {
        super();
        Image image;

        try {
            image = ImageIO.read(Objects.requireNonNull(getClass().getResource("/images/maze.png")));
            image = image.getScaledInstance((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 75), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 75), Image.SCALE_SMOOTH);
            ImageIcon icon = new ImageIcon(image);
            setIcon(icon);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony przycisku \"Labirynt\"");
        }


        setText("Labirynt");
        setPreferredSize(dimension);

        setBackground(new Color(0, 160, 255));
        setFocusPainted(false);
        setBorder(BorderFactory.createEtchedBorder());

        setHorizontalAlignment(SwingConstants.LEFT);
        addActionListener(e -> click());
    }

    private void click() {
        Core.getMainWindow().setActivePanel(PanelType.Maze);
    }
}
