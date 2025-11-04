package GUI.buttons;

import GUI.panels.SettingsPanel;
import common.Core;
import numerators.PaletteType;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

public class PaletteButton extends JButton {
// Zmienne
    private static ImageIcon icon;

    private final SettingsPanel parent;
    private final PaletteType type;
    private Color color;


    static {
    // Ustawienie niestandardowych nazw tekstów
        UIManager.put("ColorChooser.cancelText", "Anuluj");
        UIManager.put("ColorChooser.okText", "Zatwierdź");
        UIManager.put("ColorChooser.resetText", "Resetuj");
        UIManager.put("ColorChooser.previewText", "Podgląd:");
        UIManager.put("ColorChooser.sampleText", "");

        UIManager.put("ColorChooser.swatchesRecentText", "Ostatnie:");
        UIManager.put("ColorChooser.swatchesNameText", "Paleta");

        UIManager.put("ColorChooser.hsvHueText", "Odcień:");
        UIManager.put("ColorChooser.hsvSaturationText", "Nasycenie:");
        UIManager.put("ColorChooser.hsvValueText", "Wartość:");
        UIManager.put("ColorChooser.hsvTransparencyText", "Przezroczystość:");

        UIManager.put("ColorChooser.hslHueText", "Odcień:");
        UIManager.put("ColorChooser.hslSaturationText", "Nasycenie:");
        UIManager.put("ColorChooser.hslLightnessText", "Jasność:");
        UIManager.put("ColorChooser.hslTransparencyText", "Przezroczystość:");

        UIManager.put("ColorChooser.rgbRedText", "Czerwony (R):");
        UIManager.put("ColorChooser.rgbGreenText", "Zielony (G):");
        UIManager.put("ColorChooser.rgbBlueText", "Niebieski (B):");
        UIManager.put("ColorChooser.rgbAlphaText", "Alfa:");

        UIManager.put("ColorChooser.cmykCyanText", "Cyjan:");
        UIManager.put("ColorChooser.cmykMagentaText", "Magenta:");
        UIManager.put("ColorChooser.cmykYellowText", "Żółty:");
        UIManager.put("ColorChooser.cmykBlackText", "Czerń:");
        UIManager.put("ColorChooser.cmykAlphaText", "Alfa:");


    // Wczytanie ikony przycisku
        Image image;
        try {
            int scale = (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 25);

            image = ImageIO.read(Objects.requireNonNull(PaletteButton.class.getResource("/images/palette.png")));
            image = image.getScaledInstance(scale, scale, Image.SCALE_SMOOTH);
            icon = new ImageIcon(image);
        }
        catch (Exception e) {
            Core.printError("nie udało się załadować ikony przycisków do wyboru koloru");
            icon = null;
        }
    }


// Konstruktor
    public PaletteButton(PaletteType type, SettingsPanel parent) {
        super();
        this.type = type;
        this.parent = parent;

        switch(type) {
            case Path: {
                color = parent.getPathColor();
                break;
            }

            case Background: {
                color = parent.getBackgroundColor();
                break;
            }
        }

        setFocusPainted(false);
        setContentAreaFilled(false);
        setBorder(BorderFactory.createEmptyBorder());

        if (icon != null) {
            setIcon(icon);
            setPreferredSize(new Dimension(icon.getIconWidth(), icon.getIconHeight()));
        }
        else {
            setText("Wybierz kolor");
            setContentAreaFilled(true);
            setBorderPainted(true);
        }

        addActionListener(e -> click());
    }


// Metody
    private void click() {
        // Otwarcie okna do wybrania koloru
        Color selectedColor = JColorChooser.showDialog(Core.getMainWindow(), "Wybierz kolor", color);
        if (selectedColor != null && selectedColor.getRGB() != color.getRGB()) {
            color = selectedColor;

            switch(type) {
                case Path:
                    parent.setPathColor(color);
                    break;

                case Background:
                    parent.setBackgroundColor(color);
                    break;
            }

            parent.updateStatus();
        }
    }
}
