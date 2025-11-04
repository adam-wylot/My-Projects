package common;

import javax.swing.*;
import java.util.ArrayList;

public class SetsBrowser {

// Zmienne
    private ArrayList<WordList> sets;
    private WordList activeSet;


// Konstruktor
    public SetsBrowser() {
        sets = WordList.getAll();
        activeSet = null;
    }


// Gettery
    public WordList getActiveSet() { return activeSet; }
    public ArrayList<WordList> getSets() { return sets; }
    public ArrayList<Word> getSetWords() {
        if (activeSet == null)
            return new ArrayList<>();

        return new ArrayList<Word>(activeSet.getWords());
    }


// Settery
    public void setActiveSet(WordList value) { activeSet = value; }


// Metody
    public void changeWords(DefaultListModel<Object> listModel) {
        if (activeSet == null)
            return;

        ArrayList<Word> words = new ArrayList<>();

        for (int i = 0; i < listModel.getSize(); i++) {
            words.add((Word) listModel.getElementAt(i));
        }

        activeSet.setWords(words);
    }
}
