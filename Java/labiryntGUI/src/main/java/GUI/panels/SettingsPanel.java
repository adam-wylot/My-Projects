package GUI.panels;

import GUI.buttons.PaletteButton;
import common.Core;
import common.Settings;
import numerators.PaletteType;
import numerators.PanelType;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;

public class SettingsPanel extends JPanel {
    private final JPanel leftPanel;
    private final JPanel rightPanel;
    private final Font font;
    private final JButton backButton;
    private final JButton okButton;

// Zmienne
    private final JTextField wallCharTF;
    private final JTextField passCharTF;
    private final JTextField entryCharTF;
    private final JTextField exitCharTF;
    private final JTextField pathCharTF;

    private Color pathColor;
    private Color backgroundColor;


// Konstruktor
    public SettingsPanel() {
        super();

        setBackground(Settings.getBackgroundColor());
        setLayout(new BorderLayout());

        pathColor = Settings.PATH_COLOR;
        backgroundColor = Settings.getBackgroundColor();


    // Panel z ustawieniami
        JPanel mainPanel = new JPanel(new GridLayout(1, 4, (int)(Core.ScaleX() * 10), (int)(Core.ScaleY() * 10)));
        mainPanel.setOpaque(false);

        font = new Font("Arial", Font.BOLD, (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 20));
        Dimension rigidDim = new Dimension(0, (int)(Core.ScaleY() * 5));

        // Panele
        leftPanel = new JPanel();
        leftPanel.setBackground(Settings.getBackgroundColor().darker());
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS));
        mainPanel.add(leftPanel, 0);

        rightPanel = new JPanel();
        rightPanel.setOpaque(false);
        rightPanel.setLayout(new BoxLayout(rightPanel, BoxLayout.Y_AXIS));
        mainPanel.add(rightPanel, 1);


        Dimension charTFdim = new Dimension(font.getSize() + (int)(Core.ScaleX() * 2), font.getSize());
        // Znak ściany
        makeLabel("Znak ściany: ");
        wallCharTF = makeTextField("" + (char) Settings.WALL_CHAR, charTFdim, new Dimension(0, (int)(Core.ScaleY() * 4)));

        // Znak przejścia
        makeLabel("Znak przejścia: ");
        passCharTF = makeTextField("" + (char) Settings.PASS_CHAR, charTFdim, rigidDim);

        // Znak wejścia
        makeLabel("Znak wejścia: ");
        entryCharTF = makeTextField("" + (char) Settings.ENTRY_CHAR, charTFdim, rigidDim);

        // Znak wyjścia
        makeLabel("Znak wyjścia: ");
        exitCharTF = makeTextField("" + (char) Settings.EXIT_CHAR, charTFdim, rigidDim);

        // Znak ścieżki
        makeLabel("Znak ścieżki: ");
        pathCharTF = makeTextField("" + (char) Settings.PATH_CHAR, charTFdim, rigidDim);


        /// PRZERWA
        leftPanel.add(Box.createRigidArea(new Dimension(0, (int)(Core.ScaleY() * 30))));
        rightPanel.add(Box.createRigidArea(new Dimension(0, (int)(Core.ScaleY() * 30))));


        PaletteButton pb;
        // Kolor ścieżki
        makeLabel("Kolor ścieżki: ");
        pb = new PaletteButton(PaletteType.Path, this);
        pb.setAlignmentX(Component.LEFT_ALIGNMENT);
        rightPanel.add(pb);

        // Kolor tła programu
        makeLabel("Kolor tła programu: ");
        pb = new PaletteButton(PaletteType.Background, this);
        pb.setAlignmentX(Component.LEFT_ALIGNMENT);
        rightPanel.add(pb);

        this.add(mainPanel, BorderLayout.CENTER);

    // Panel z przyciskami
        JPanel buttonPanel = new JPanel();
        buttonPanel.setOpaque(false);
        buttonPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));

        JPanel tmpPanel = new JPanel(new BorderLayout());
        tmpPanel.setOpaque(false);
        tmpPanel.add(buttonPanel, BorderLayout.SOUTH);
        mainPanel.add(new JLabel(" "), 2);
        mainPanel.add(tmpPanel, 3);


    // Przycisk Powrót
        backButton = new JButton("Powrót");
        backButton.setFocusPainted(false);
        backButton.addActionListener(e -> {
            Core.getMainWindow().setActivePanel(PanelType.Maze);
            System.gc();
        });
        buttonPanel.add(backButton);

        okButton = new JButton("Zatwierdź");
        okButton.setFocusPainted(false);
        okButton.setEnabled(false);
        okButton.addActionListener(e -> setSettings());
        buttonPanel.add(okButton);
    }


// Gettery
    public Color getPathColor() { return pathColor; }
    public Color getBackgroundColor() { return backgroundColor; }


// Settery
    public void setPathColor(Color color) {
        pathColor = new Color(color.getRed(), color.getGreen(), color.getBlue());
    }
    public void setBackgroundColor(Color color) {
        backgroundColor = new Color(color.getRed(), color.getGreen(), color.getBlue());
    }


// Metody
    private void makeLabel(String text) {
        JLabel pathCharLabel = new JLabel(text);
        pathCharLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
        pathCharLabel.setFont(font);
        leftPanel.add(pathCharLabel);
    }

    private JTextField makeTextField(String text, Dimension maxSizeDim, Dimension gapDim) {
        JTextField textField = new JTextField();
        textField.setAlignmentX(Component.LEFT_ALIGNMENT);
        textField.setText(text);
        textField.setMaximumSize(maxSizeDim);
        textField.setPreferredSize(maxSizeDim);
        textField.setBorder(BorderFactory.createEtchedBorder());

        textField.getDocument().addDocumentListener(new DocumentListener() {
            public void changedUpdate(DocumentEvent e) {
                updateStatus();
            }
            public void removeUpdate(DocumentEvent e) {
                updateStatus();
            }
            public void insertUpdate(DocumentEvent e) {
                updateStatus();
            }
        });

        rightPanel.add(Box.createRigidArea(gapDim));
        rightPanel.add(textField);
        return textField;
    }

    public void updateStatus() {
        if (wallCharTF.getText().length() == 1 && wallCharTF.getText().charAt(0) != Settings.WALL_CHAR) {
            okButton.setEnabled(true);
        }
        else if (passCharTF.getText().length() == 1 && passCharTF.getText().charAt(0) != Settings.PASS_CHAR) {
            okButton.setEnabled(true);
        }
        else if (entryCharTF.getText().length() == 1 && entryCharTF.getText().charAt(0) != Settings.ENTRY_CHAR) {
            okButton.setEnabled(true);
        }
        else if (exitCharTF.getText().length() == 1 && exitCharTF.getText().charAt(0) != Settings.EXIT_CHAR) {
            okButton.setEnabled(true);
        }
        else if (pathCharTF.getText().length() == 1 && pathCharTF.getText().charAt(0) != Settings.PATH_CHAR) {
            okButton.setEnabled(true);
        }
        else if (pathColor.getRGB() != Settings.PATH_COLOR.getRGB()) {
            okButton.setEnabled(true);
        }
        else if (backgroundColor.getRGB() != Settings.getBackgroundColor().getRGB()) {
            okButton.setEnabled(true);
        }
        else {
            okButton.setEnabled(false);
        }
    }

    private void setSettings() {
    // Sprawdzenie wartości
        if (wallCharTF.getText().length() != 1 || wallCharTF.getText().charAt(0) > 255) {
            JOptionPane.showMessageDialog(Core.getMainWindow(), "Nieprawidłowa wartość dla znaku ściany.", "Błąd!", JOptionPane.ERROR_MESSAGE);
            return;
        }

        if (passCharTF.getText().length() != 1 || passCharTF.getText().charAt(0) > 255) {
            JOptionPane.showMessageDialog(Core.getMainWindow(), "Nieprawidłowa wartość dla znaku przejścia.", "Błąd!", JOptionPane.ERROR_MESSAGE);
            return;
        }

        if (entryCharTF.getText().length() != 1 || entryCharTF.getText().charAt(0) > 255) {
            JOptionPane.showMessageDialog(Core.getMainWindow(), "Nieprawidłowa wartość dla znaku wejścia.", "Błąd!", JOptionPane.ERROR_MESSAGE);
            return;
        }

        if (exitCharTF.getText().length() != 1 || exitCharTF.getText().charAt(0) > 255) {
            JOptionPane.showMessageDialog(Core.getMainWindow(), "Nieprawidłowa wartość dla znaku wyjścia.", "Błąd!", JOptionPane.ERROR_MESSAGE);
            return;
        }

        if (pathCharTF.getText().length() != 1 || pathCharTF.getText().charAt(0) > 255) {
            JOptionPane.showMessageDialog(Core.getMainWindow(), "Nieprawidłowa wartość dla znaku ścieżki.", "Błąd!", JOptionPane.ERROR_MESSAGE);
            return;
        }


    // Ustawianie wartości
        Settings.WALL_CHAR = (byte) wallCharTF.getText().charAt(0);
        Settings.PASS_CHAR = (byte) passCharTF.getText().charAt(0);
        Settings.ENTRY_CHAR = (byte) entryCharTF.getText().charAt(0);
        Settings.EXIT_CHAR = (byte) exitCharTF.getText().charAt(0);
        Settings.PATH_CHAR = (byte) pathCharTF.getText().charAt(0);

        Settings.PATH_COLOR = new Color(pathColor.getRed(), pathColor.getGreen(), pathColor.getBlue());
        Settings.setBackgroundColor(new Color(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue()));
        Core.getMainWindow().getMazePanel().getCanvasPanel().changeBackgroundColor(Settings.getBackgroundColor());


        // zmiana koloru tła aktywnego okna ustawień
        setBackground(Settings.getBackgroundColor());
        leftPanel.setBackground(Settings.getBackgroundColor().darker());

        this.revalidate();
        this.repaint();


        okButton.setEnabled(false);
    }
}
