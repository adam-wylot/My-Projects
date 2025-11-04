package GUI;

import common.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public abstract class WordWindow extends JDialog implements ActionListener {
// Elementy
    protected JPanel gridPanel;

    protected JButton acceptButton;
    protected JTextField enWordTF;
    protected JTextField plWordTF;
    protected JTextField categoryTF;
    protected JTextField defTF;


// Konstruktor
    public WordWindow(int width, int height) {
        super(Core.Instance().getMainWindow(), "Nowe okno", Dialog.ModalityType.APPLICATION_MODAL);

        setSize(width, height);
        setLayout(new BorderLayout());
        setResizable(false);   // brak możliwości zmiany wielkości okna
        setLocationRelativeTo(null);   // wycentrowanie okna
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        setBackground(Core.Instance().getProgramColor());

        // Dodanie przycisku zamykającego nowe okno
        acceptButton = new JButton();
        acceptButton.setFocusPainted(false);
        acceptButton.addActionListener(this);
        acceptButton.setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 32f));
        ///acceptButton.setPreferredSize(new Dimension(width, (int)(Core.Instance().getVerScale() * 60)));
        acceptButton.setBackground(Color.lightGray);
        this.add(acceptButton, BorderLayout.SOUTH);

        // Dodanie pól tekstowych
        // Tworzenie panelu
        gridPanel = new JPanel(new GridLayout(0, 2));

        // Tworzenie pól tekstowych



        enWordTF = new JTextField();
        gridPanel.add(new JLabel("Angielskie słówko:"));
        gridPanel.add(enWordTF);

        plWordTF = new JTextField();
        gridPanel.add(new JLabel("Tłumaczenie:"));
        gridPanel.add(plWordTF);

        categoryTF = new JTextField();
        gridPanel.add(new JLabel("Kategoria:"));
        gridPanel.add(categoryTF);

        defTF = new JTextField();
        gridPanel.add(new JLabel("Definicja:"));
        gridPanel.add(defTF);

        // Dodanie panelu do ramki
        this.add(gridPanel, BorderLayout.CENTER);
    }


// Metody
    @Override
    public abstract void actionPerformed(ActionEvent e);
}
