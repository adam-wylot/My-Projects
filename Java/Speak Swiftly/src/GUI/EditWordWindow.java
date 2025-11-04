package GUI;

import common.Category;
import common.Core;
import common.Word;
import common.WordList;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.ArrayList;


public class EditWordWindow extends WordWindow {

// Elementy
    private final JComboBox<WordList> wordListchooser;
    private final JComboBox<Object> findWord;


// Konstruktor
    public EditWordWindow(int width, int height) {
        super(width, height);

        this.setTitle("Edytuj słówko");
        acceptButton.setText("Zapisz");

    // Dodanie pól tekstowych
        wordListchooser = new JComboBox<>(WordList.getAll().toArray(new WordList[0]));
        wordListchooser.setSelectedIndex(0);
        wordListchooser.addActionListener(this);
        gridPanel.add(new JLabel("Wybierz zestaw:"), 0);
        gridPanel.add(wordListchooser, 1);

        findWord = new JComboBox<>();
        findWord.addActionListener(this);
        gridPanel.add(new JLabel("Wybierz słówko:"), 2);
        gridPanel.add(findWord, 3);

        // dodawanie opcji
        findWord.addItem("-nie wybrano-");
        ArrayList<Word> list = (ArrayList<Word>) WordList.getMasterList().getWords();

        for (Word word : list) {
            findWord.addItem(word);
        }

        // wyłączenie niepotrzebnych opcji
        acceptButton.setEnabled(false);
        enWordTF.setEnabled(false);
        plWordTF.setEnabled(false);
        categoryTF.setEnabled(false);
        defTF.setEnabled(false);

        this.setVisible(true);
    }


    // Metody
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == wordListchooser && wordListchooser.getSelectedItem() instanceof WordList wordList) {
            findWord.removeAllItems();

            findWord.addItem("-nie wybrano-");
            for (Word word : wordList.getWords()) {
                findWord.addItem(word);
            }

            findWord.revalidate();
        }
        else if (e.getSource() == findWord) {
            if (findWord.getSelectedItem() instanceof Word word) {
                acceptButton.setEnabled(true);
                enWordTF.setEnabled(true);
                enWordTF.setText(word.getWordEN());
                enWordTF.setEditable(true);

                plWordTF.setEnabled(true);
                plWordTF.setText(word.getWordPL());
                plWordTF.setEditable(true);

                categoryTF.setEnabled(true);
                categoryTF.setText(word.getCategory(0).getCategoryName());
                categoryTF.setEditable(true);

                defTF.setEnabled(true);
                defTF.setText(word.getDef());
                defTF.setEditable(true);
            }
            else if (findWord.getSelectedItem() instanceof String str && str.equals("-nie wybrano-")) {
                acceptButton.setEnabled(false);
                enWordTF.setEnabled(false);
                enWordTF.setText("");
                plWordTF.setEnabled(false);
                plWordTF.setText("");
                categoryTF.setEnabled(false);
                categoryTF.setText("");
                defTF.setEnabled(false);
                defTF.setText("");
            }
        }
        else if (e.getSource() == acceptButton) {
            // Edycja słówka
            Word word;

            String en = enWordTF.getText();
            String pl = plWordTF.getText();
            String cat = categoryTF.getText();
            String def = defTF.getText();

            if(en.isEmpty() || pl.isEmpty() || cat.isEmpty())
                return;

            if(def.isEmpty()) {
                word = new Word(en, pl);
            }
            else {
                word = new Word(en, pl, def);
            }

            word.addCategory( Category.addCategory(cat) );

            for (Word words : WordList.getMasterList().getWords()) {
                if (words.getWordEN() == findWord.getSelectedItem()) {
                    int wordID = words.getId();

                    WordList.getMasterList().alterWordENG(wordID, en);
                    WordList.getMasterList().alterWordPL(wordID, pl);
                    WordList.getMasterList().alterWordDefinition(wordID, def);
                    //Aktualnie kategorii nie można zmienić, gdyż są listą, z tego co widzę (i nie ma do niej tutaj aż tak łatwego gettera?)
                    //Zatem, trudno... można usunąć słowo i dodać nowe, ale wtedy ignoruję funkcje alterWord
                }
            }


            Core.printLog("dokonano edycji słówka: " + word);
            Core.Instance().getMainWindow().getMainPanel().resetElements();    // zresetowanie fiszek

            // Zamknięcie okna
            //TEMPORARY
            this.dispose();
        }
    }
}
