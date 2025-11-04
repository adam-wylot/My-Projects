package GUI.buttons;

import common.Core;
import common.Flashcards;
import common.Word;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class FlashcardButton extends JButton implements ActionListener {
// Zmienne
    private boolean isReversed;


// Konstruktor
    public FlashcardButton() {
        setBackground(Color.WHITE);
        setBorderPainted(true);
        setFocusPainted(false);
        setOpaque(true);
        addActionListener(this);
        setBorder(BorderFactory.createLineBorder(Color.BLACK, (int)(Math.max(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 5)));
        setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 40f));

        isReversed = false;

        Core.Instance().getFlashcards().reset();
        if (Core.Instance().getFlashcards().getWordsCount() > 0) {
            this.setText(Core.Instance().getFlashcards().getFront());
        }
    }


// Settery
    public void setReversed(boolean value) { isReversed = value; }


// Metody
    @Override
    public void actionPerformed(ActionEvent e) {
        isReversed = !isReversed;
        setFlashcard();
    }

    public void setFlashcard() {
        if (isReversed) {
            this.setText(Core.Instance().getFlashcards().getBack());
        }
        else {
            this.setText(Core.Instance().getFlashcards().getFront());
        }
    }
}
