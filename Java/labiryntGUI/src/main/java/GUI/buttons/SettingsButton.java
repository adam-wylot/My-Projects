package GUI.buttons;

import common.Core;
import numerators.PanelType;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

public class SettingsButton extends JButton {
    // Zmienne
    private ImageIcon icon;

    // Konstruktor
    public SettingsButton(Dimension dimension) {
        super();
        Image image;

        try {
            image = ImageIO.read(Objects.requireNonNull(getClass().getResource("/images/settings.png")));

            image = image.getScaledInstance((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 35), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 35), Image.SCALE_SMOOTH);
            icon = new ImageIcon(image);
            setIcon(icon);

            setBorderPainted(false);
            setPreferredSize(dimension);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony przycisku z ustawieniami programu");
            setText("Ustawienia");
            setBorder(BorderFactory.createEtchedBorder());
            setBorderPainted(true);

            setPreferredSize(new Dimension(dimension.width * 2, dimension.height));
        }




        setContentAreaFilled(false);
        setFocusPainted(false);


        setHorizontalAlignment(SwingConstants.CENTER);

        // funkcjonalność przycisku
        addActionListener(e -> click());
    }


    // Metody
    private void click() {
        Core.getMainWindow().setActivePanel(PanelType.Settings);
    }
}
