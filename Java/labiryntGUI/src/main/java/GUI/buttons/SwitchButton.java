package GUI.buttons;

import common.Core;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Objects;

public class SwitchButton extends JButton {
    private final static ArrayList<SwitchButton> instances = new ArrayList<>();

    protected String text;
    protected boolean active;
    protected ImageIcon inactiveIcon;
    protected ImageIcon activeIcon;
    protected Color inactiveColor;
    protected Color activeColor;


// Metody
    public SwitchButton(String Text, Dimension dim, String inactivePath, String activePath, Color inactiveColor, Color activeColor) {
        super();

        instances.add(this);
        this.text = Text;
        active = false;
        this.inactiveColor = inactiveColor;
        this.activeColor = activeColor;

        Image image;

    // wczytanie obrazków
        // obraz nieaktywnego przycisku
        try {
            image = ImageIO.read(Objects.requireNonNull(getClass().getResource(inactivePath)));
            image = image.getScaledInstance((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 50), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 50), Image.SCALE_SMOOTH);
            inactiveIcon = new ImageIcon(image);
            setIcon(inactiveIcon);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony dla nieaktywnego \"Switch Button\"");
        }

        // obraz aktywnego przycisku
        try {
            image = ImageIO.read(Objects.requireNonNull(getClass().getResource(activePath)));
            image = image.getScaledInstance((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 50), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 50), Image.SCALE_SMOOTH);
            activeIcon = new ImageIcon(image);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony dla aktywnego \"Switch Button\"");
        }

        setPreferredSize(dim);
        setText(Text);
        setBackground(inactiveColor);
        setOpaque(true);
        setFocusPainted(false);
        setBorder(BorderFactory.createEtchedBorder());

        setHorizontalAlignment(SwingConstants.LEFT);

        // funkcjonalność przycisku
        addActionListener(e -> click());
        setVisible(false);
    }


// Metody
    private void click() {
        setActive(!active);
    }

    public void setActive(boolean active) {
        if (active) {
            deactivateAll();

            // ustawienie 'X'
            this.active = true;

            setIcon(activeIcon);
            if (activeIcon != null) {
                setText("");
            }
            else {
                setText("Cofnij");
            }

            setBackground(activeColor);
            setHorizontalAlignment(SwingConstants.CENTER);
        }
        else {
            // ustawienie flagi
            this.active = false;

            setIcon(inactiveIcon);
            setBackground(inactiveColor);
            setText(text);
            setHorizontalAlignment(SwingConstants.LEFT);
        }
    }

    public boolean isActive() { return active; }

    private void deactivateAll() {
        for (SwitchButton sb : instances) {
            sb.setActive(false);
        }
    }
}
