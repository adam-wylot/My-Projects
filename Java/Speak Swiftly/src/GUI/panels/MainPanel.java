package GUI.panels;

import GUI.buttons.FlashcardButton;
import common.Core;

import javax.swing.*;
import java.awt.*;

public class MainPanel extends JPanel {
// Elementy
    private FlashcardButton flashcardButton;
    private TopPanel topPanel;
    private BottomPanel bottomPanel;


// Konstruktor
    public MainPanel(int width, int height) {
        setBackground(Core.Instance().getProgramColor());
        setLayout(new BorderLayout());

    // Dodanie paneli
        int flashcardSize = (int)(0.8 * Math.min(width, height));
        JPanel tmpPanel;

        /// ŚRODEK — fiszki
        flashcardButton = new FlashcardButton();
        this.add(flashcardButton, BorderLayout.CENTER);

        /// GÓRA
        topPanel = new TopPanel(width, (height - flashcardSize) / 2);
        this.add(topPanel, BorderLayout.NORTH);

        /// DÓŁ
        bottomPanel = new BottomPanel(width, (height - flashcardSize) / 2);
        this.add(bottomPanel, BorderLayout.SOUTH);

        /// LEWO
        tmpPanel = new JPanel();
        tmpPanel.setOpaque(false);
        tmpPanel.setPreferredSize(new Dimension((width - flashcardSize) / 2, height));
        this.add(tmpPanel, BorderLayout.WEST);

        // PRAWO
        tmpPanel = new JPanel();
        tmpPanel.setOpaque(false);
        tmpPanel.setPreferredSize(new Dimension((width - flashcardSize) / 2, height));
        this.add(tmpPanel, BorderLayout.EAST);
    }


// Gettery
    public FlashcardButton getFlashcardButton() { return flashcardButton; }
    public TopPanel getTopPanel() { return topPanel; }


// Metody
    public void resetElements() {
        Core.Instance().getFlashcards().reset();
        topPanel.setCounter();
        flashcardButton.setFlashcard();
    }
}
