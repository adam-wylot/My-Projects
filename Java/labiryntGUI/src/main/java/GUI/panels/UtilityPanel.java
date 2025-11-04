package GUI.panels;

import common.Core;

import javax.swing.*;
import java.awt.*;

public class UtilityPanel extends JPanel {
// Zmienne
    private Color color;
    private JButton zoomIn;
    private JButton zoomOut;
    private JLabel zoomLabel;
    private JLabel positionLabel;
    private int zoom;


// Konstruktor
    public UtilityPanel(int width, int height) {
        super();

        color = new Color(41, 244, 153);
        zoom = 100;

        this.setLayout(new BorderLayout());
        this.setPreferredSize(new Dimension(width, height));
        this.setBackground(color);

    // konstrukcja
        JPanel leftPanel = new JPanel();
        leftPanel.setOpaque(false);
        leftPanel.setLayout(new FlowLayout(FlowLayout.LEFT, (int)(Core.ScaleX() * 5), 0));
        this.add(leftPanel, BorderLayout.WEST);

        JPanel rightPanel = new JPanel();
        rightPanel.setOpaque(false);
        rightPanel.setLayout(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        this.add(rightPanel, BorderLayout.EAST);

        positionLabel = new JLabel("(0, 0)");
        positionLabel.setFont(new Font("Arial", Font.ITALIC, 13));
        positionLabel.setForeground(Color.GRAY);
        leftPanel.add(positionLabel);

        zoomLabel = new JLabel("100%  ");
        zoomLabel.setFont(new Font("Arial", Font.BOLD, 13));
        zoomLabel.setForeground(Color.BLACK);
        rightPanel.add(zoomLabel);


    // Przyciski
        //Image image;

        // Zoom In
        zoomIn = new JButton();
        zoomIn.setBackground(color);
        zoomIn.setFocusPainted(false);
        zoomIn.setBorder(BorderFactory.createEtchedBorder());

        zoomIn.setText("+");
        zoomIn.setFont(new Font("Arial", Font.BOLD, 15));
        zoomIn.setPreferredSize(new Dimension(15, 15));
        zoomIn.setHorizontalTextPosition(SwingConstants.CENTER);
        zoomIn.setVerticalTextPosition(SwingConstants.CENTER);


        zoomIn.addActionListener(e -> {
            Core.getMainWindow().getMazePanel().zoom(0.25);
            if (zoom + 25 <= 500) {
                zoom += 25;
                setZoomText();
            }
        });
        rightPanel.add(zoomIn);


        // Zoom Out
        zoomOut = new JButton();
        zoomOut.setBackground(color);
        zoomOut.setFocusPainted(false);
        zoomOut.setBorder(BorderFactory.createEtchedBorder());

        zoomOut.setText("–");
        zoomOut.setFont(new Font("Arial", Font.BOLD, 15));
        zoomOut.setPreferredSize(new Dimension(15, 15));
        zoomOut.setHorizontalTextPosition(SwingConstants.CENTER);
        zoomOut.setVerticalTextPosition(SwingConstants.CENTER);

        zoomOut.addActionListener(e -> {
            Core.getMainWindow().getMazePanel().zoom(-0.25);
            if (zoom - 25 > 0) {
                zoom -= 25;
                setZoomText();
            }
        });
        rightPanel.add(zoomOut);
    }


// Metody
    private void setZoomText() {
        zoomLabel.setText(zoom + "%  ");
    }

    public void setXYlabel(int x, int y) {
        positionLabel.setText("(" + x + ", " + y + ")");
    }
}
