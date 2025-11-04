package GUI.panels;

import GUI.buttons.*;
import common.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;

public class ToolsPanel extends JPanel {
// Zmienne
    private MazeButton mazeButton;
    private FindPathButton findPathButton;
    private SwitchButton setStartButton;
    private SwitchButton setEndButton;
    private SettingsButton settingsButton;


// Konstruktor
    public ToolsPanel(int width, int height) {
        super();

        setPreferredSize(new Dimension(width, height));
        setLayout(new BorderLayout());

    // Ustawianie przycisków
        JPanel flowLayoutP = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, (int)(Core.ScaleY() * 3)));
        flowLayoutP.setOpaque(false);
        this.add(flowLayoutP, BorderLayout.NORTH);
        flowLayoutP.setPreferredSize(new Dimension(width, (int)(Core.ScaleY() * (75 + 75 + 60 + 3*3))));

        /// GÓRA
        Dimension dim = new Dimension(width - (int)(Core.ScaleX() * 10), (int)(Core.ScaleY() * 75));
        mazeButton = new MazeButton(dim);
        flowLayoutP.add(mazeButton);

        findPathButton = new FindPathButton(dim);
        flowLayoutP.add(findPathButton);

        dim = new Dimension((width - (int)(Core.ScaleX() * 10)) / 2, (int)(Core.ScaleY() * 60));
        Color c1 = new Color(6, 170, 240);
        Color c2 = new Color(185, 185, 185);
        setStartButton = new SwitchButton("<html>Wybierz<br>wejście", dim, "/images/start_flag.png", "/images/cancel.png", c1, c2);
        flowLayoutP.add(setStartButton);

        setEndButton = new SwitchButton("<html>Wybierz<br>wyjście", dim, "/images/end_flag.png", "/images/cancel.png", c1, c2);
        flowLayoutP.add(setEndButton);

        /// DÓŁ
        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.setBorder(BorderFactory.createEtchedBorder());
        bottomPanel.setBackground(new Color(41, 244, 153));
        bottomPanel.setOpaque(false);
        this.add(bottomPanel, BorderLayout.SOUTH);

        settingsButton = new SettingsButton(new Dimension((int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 35), (int)(Math.max(Core.ScaleX(), Core.ScaleY()) * 35)));
        bottomPanel.add(settingsButton, BorderLayout.WEST);
    }


    // Gettery
    public SwitchButton getStartButton() { return setStartButton; }
    public SwitchButton getEndButton() { return setEndButton; }
    public FindPathButton getFindPathButton() { return findPathButton; }


    // Metody
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        int width = getWidth();
        int height = getHeight();

        // ustawianie gradientu
        GradientPaint gradient = new GradientPaint(0, 0, new Color(0, 158, 253), width, height, new Color(42, 245, 152));

        // ustawianie wcześniej przygotowanego gradientu jako "pędzel"
        Graphics2D g2d = (Graphics2D) g;
        g2d.setPaint(gradient);

        // rysowanie przygotowanego wcześniej gradientu na panelu
        g2d.fill(new Rectangle2D.Double(0, 0, width, height));
    }
}
