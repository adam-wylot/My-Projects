package common;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Collections;

public class Flashcards {

// Zmienne
    private final ArrayList<Word> words;
    private boolean ENtoPL;
    private int wordNumber;
    private int wordsCount;


// Konstruktor
    public Flashcards() {
        words = new ArrayList<>();
        ENtoPL = true;
        wordNumber = 0;
        wordsCount = 0;
    }


// Gettery
    public String getFront() {
        if (wordNumber >= wordsCount) {
            return "";
        }


        if (ENtoPL) {
            return words.get(wordNumber).getWordEN();
        }
        return words.get(wordNumber).getWordPL();
    }

    public String getBack() {
        if (wordNumber >= wordsCount) {
            return "";
        }


        if (ENtoPL) {
            return words.get(wordNumber).getWordPL();
        }
        return words.get(wordNumber).getWordEN();
    }

    public int getWordNumber() { return wordNumber; }
    public int getWordsCount() { return wordsCount; }


// Settery
    public void setENtoPL(boolean value) { ENtoPL = value; }


// Metody


    public void reset() {
        words.clear();
        wordNumber = 0;

        for (int i = 0; i < WordList.count(); i++) {
            words.addAll(WordList.get(i).getWords());
        }

        wordsCount = words.size();
        if (wordsCount == 0) {
            return;
        }

        Collections.shuffle(words);
    }

    public void next() {
        if (wordNumber + 1 >= wordsCount) {
            JOptionPane.showMessageDialog(null, "Skończyły ci się fiszki ☺", Core.programName, JOptionPane.INFORMATION_MESSAGE);
            return;
        }

        wordNumber++;
    }

    public void back() {
        if (wordNumber - 1 >= 0) {
            wordNumber--;
        }
    }
}
