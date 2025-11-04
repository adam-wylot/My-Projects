package GUI;

import javax.swing.*;

import GUI.panels.MainPanel;
import GUI.panels.MenuPanel;
import common.Core;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class MainWindow extends JFrame {
// Zmienne
    // Elementy
    private final ImageIcon appIcon;
    private final MenuPanel menuPanel;
    private final MainPanel mainPanel;


// Konstruktory
    public MainWindow(int width, int height) {
        super();

    // Okno
        setTitle(Core.programName);
        setBackground(Core.Instance().getProgramColor());
        setSize(width, height);
        setResizable(false);
        setLocationRelativeTo(null);   // wycentrowanie okna
        setLayout(new BorderLayout());
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        setClosingAction();


    // Elementy
        // Ikonka
        appIcon = new ImageIcon("assets\\images\\light_icon.png");
        setIconImage(appIcon.getImage());


        // Tworzenie panelu
        menuPanel = new MenuPanel(this.getWidth() / 4, this.getHeight());
        this.add(menuPanel, BorderLayout.WEST);

        // druga kolumna
        mainPanel = new MainPanel(this.getWidth() - (this.getWidth() / 4), this.getHeight());
        this.add(mainPanel, BorderLayout.CENTER);

        this.setVisible(true);
    }

    public MainWindow() {
        this( (int)(1600 * Core.Instance().getHorScale()), (int)(900 * Core.Instance().getVerScale()) );
    }


// Gettery
    public MainPanel getMainPanel() { return mainPanel; }


// Metody
    private void setClosingAction() {
        JFrame frame = this;
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                Core.end();

                // Zamknięcie
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.dispose();
            }
        });
    }
}
