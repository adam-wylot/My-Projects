package GUI.panels;

import common.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class TopPanel extends JPanel implements ActionListener {

// Elementy
    private JComboBox<String> option;
    private JButton resetButton;
    private JLabel flashcardNumberLabel;
    private JLabel flashcardsCountLabel;


// Konstruktor
    public TopPanel(int width, int height) {
        super();

        setLayout(new FlowLayout(FlowLayout.LEFT, (int)(Core.Instance().getHorScale() * 5), 0));
        setPreferredSize(new Dimension(width, height));
        setOpaque(false);


        option = new JComboBox<>(new String[]{"angielski → polski", "polski → angielski"});
        option.setFocusable(false);
        option.setBackground(Core.Instance().getProgramColor().darker());
        option.setForeground(Color.WHITE);
        option.setPreferredSize(new Dimension((int)(Core.Instance().getHorScale() * 270), (int)(Core.Instance().getHorScale() * 45)));
        option.setBorder(BorderFactory.createEtchedBorder());
        option.addActionListener(this);
        if (System.getProperty("os.name").toLowerCase().contains("mac")) {
            // System to macOS i nic na nim nie działa normalnie, więc musi być tu if
            option.setBackground(Color.WHITE);
            option.setForeground(Color.BLACK);
            option.setBorder(BorderFactory.createEmptyBorder());
        }

        this.add(option);

        resetButton = new JButton("Reset");
        resetButton.setFocusable(false);
        resetButton.setPreferredSize(new Dimension((int)(Core.Instance().getHorScale() * 75), (int)(Core.Instance().getHorScale() * 45)));
        resetButton.setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 16f));
        resetButton.setBackground(Core.Instance().getProgramColor().darker());
        resetButton.setBorder(BorderFactory.createEtchedBorder());
        resetButton.setForeground(Color.WHITE);
        resetButton.addActionListener(this);
        this.add(resetButton);

        JLabel tmpLabel = new JLabel(" ");
        tmpLabel.setPreferredSize(new Dimension((int)(Core.Instance().getHorScale() * 700), 1));
        this.add(tmpLabel);



    // Licznik
        Font counterFont = Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) *40f);

        flashcardNumberLabel = new JLabel("-");
        flashcardNumberLabel.setForeground(Color.WHITE);
        flashcardNumberLabel.setFont(counterFont);
        this.add(flashcardNumberLabel);

        tmpLabel = new JLabel("/");
        tmpLabel.setForeground(Color.WHITE);
        tmpLabel.setFont(counterFont);
        this.add(tmpLabel);

        flashcardsCountLabel = new JLabel("-");
        flashcardsCountLabel.setForeground(Color.WHITE);
        flashcardsCountLabel.setFont(counterFont);
        this.add(flashcardsCountLabel);

        setCounter();
    }


// Metody
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == option) {
            // OPCJA JĘZYKOWA

            switch(option.getSelectedIndex()) {
                case 0: {
                    Core.Instance().getFlashcards().setENtoPL(true);
                    break;
                }
                case 1: {
                    Core.Instance().getFlashcards().setENtoPL(false);
                    break;
                }
            }
        }
        else if (e.getSource() == resetButton) {
            // PRZYCISK RESET
            Core.Instance().getFlashcards().reset();
            setCounter();
        }


        if (this.getParent() != null && this.getParent() instanceof MainPanel mainPanel) {
            mainPanel.getFlashcardButton().setFlashcard();
        }
    }

    public void setCounter() {
        if (Core.Instance().getFlashcards().getWordsCount() == 0) {
            flashcardNumberLabel.setText("-");
            flashcardsCountLabel.setText("-");

            return;
        }

        flashcardNumberLabel.setText("" + (Core.Instance().getFlashcards().getWordNumber() + 1));
        flashcardsCountLabel.setText("" + Core.Instance().getFlashcards().getWordsCount());
    }
}
