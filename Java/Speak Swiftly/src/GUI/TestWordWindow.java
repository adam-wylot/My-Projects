package GUI;

import common.Core;
import common.Word;
import common.WordList;


import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
public class TestWordWindow extends JDialog implements ActionListener  {

// Zmienne
    private final List<Word> wordsToTest;
    private int currentWordIndex;
    private int score;


// Elementy
    private final JLabel wordLabel;
    private final JTextField answerField;
    private final JButton nextButton;


// Konstruktor
    public TestWordWindow(int width, int height) {
        super(Core.Instance().getMainWindow(), "Test ze słówek", Dialog.ModalityType.APPLICATION_MODAL);

        this.setSize(width, height);
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        this.setLayout(new GridLayout(3, 1));

        wordsToTest = new ArrayList<>();
        WordList wordList;

    // Pobranie słów do testu i sprawdzenie, czy w ogóle jakieś są
        for(int i = 0; i < WordList.count(); i++){
            wordList = WordList.get(i);

            for(int j = 0; j < wordList.getWordsSize(); j++){
                wordsToTest.add(wordList.getWords().get(j));
            }
        }

        if(wordsToTest.isEmpty()){
            JOptionPane.showMessageDialog(null, "Brak dodanych słówek do listy fiszek!", "Błąd", JOptionPane.ERROR_MESSAGE);
            this.dispose();
        }


    // znalazło słówka, więc je tasuje
        Collections.shuffle(wordsToTest);

    // inicjalizacja zmiennych
        currentWordIndex = 0;
        score = 0;

    // Elementy GUI
        wordLabel = new JLabel("Podaj tłumaczenie słowa: " + wordsToTest.get(currentWordIndex).getWordEN(), SwingConstants.CENTER);
        answerField = new JTextField();
        nextButton = new JButton("Dalej");
        nextButton.addActionListener(this);

        // Dodanie elementów do okna
        this.add(wordLabel);
        this.add(answerField);
        this.add(nextButton);

        this.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == nextButton) {
            String answer = answerField.getText().trim();
            Word currentWord = wordsToTest.get(currentWordIndex);

            if (answer.equalsIgnoreCase(currentWord.getWordPL().toLowerCase())) {
                score++;
            }

            currentWordIndex++;

            if (currentWordIndex < wordsToTest.size()) {
                wordLabel.setText("Podaj tłumaczenie słowa: " + wordsToTest.get(currentWordIndex).getWordEN());
                answerField.setText("");
            }
            else {
                Core.printLog("ukończono test z wynikiem: " + + (double) score / currentWordIndex * 100 + "%  [" + score + "/" + currentWordIndex + "]");
                JOptionPane.showMessageDialog(this, "Twój wynik to: " + (double) score / currentWordIndex * 100 + "%  [" + score + "/" + currentWordIndex + "].", "Wynik testu", JOptionPane.INFORMATION_MESSAGE);
                this.dispose();
            }
        }
    }
}
