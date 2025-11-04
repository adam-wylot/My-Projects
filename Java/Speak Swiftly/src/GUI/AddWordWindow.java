package GUI;

import common.Category;
import common.Core;
import common.Word;
import common.WordList;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.ArrayList;

public class AddWordWindow extends WordWindow {

// Zmienne
    private final JComboBox<Object> setDropdown;


// Konstruktor
    public AddWordWindow(int width, int height) {
        super(width, height);

        setTitle("Dodaj słówko");
        acceptButton.setText("Dodaj");

        setDropdown = new JComboBox<>();
        setDropdown.addItem("-brak-");

        ArrayList<WordList> wordLists = WordList.getAll();
        for (WordList wordList : wordLists) {
            setDropdown.addItem(wordList);
        }
        setDropdown.removeItem(WordList.getMasterList());


        gridPanel.add(new JLabel("Wybierz zestaw:"), 4);
        gridPanel.add(setDropdown, 5);

        this.setVisible(true);
    }


// Metody
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == acceptButton) {
            // Dodanie słówka
            Word word;

            String en = enWordTF.getText();
            String pl = plWordTF.getText();
            String cat = categoryTF.getText();
            String def = defTF.getText();
            Object set = setDropdown.getSelectedItem();

            if(en.isEmpty()) {
                JOptionPane.showMessageDialog(null, "Pole z angielską wersją słówka musi być uzupełnione", "Błąd", JOptionPane.ERROR_MESSAGE);
                return;
            }
            if (pl.isEmpty()) {
                JOptionPane.showMessageDialog(null, "Pole z polską wersją słówka musi być uzupełnione", "Błąd", JOptionPane.ERROR_MESSAGE);
                return;
            }
            if (cat.isEmpty()) {
                JOptionPane.showMessageDialog(null, "Pole z kategorią słówka musi być uzupełnione", "Błąd", JOptionPane.ERROR_MESSAGE);
                return;
            }



            if(def.isEmpty()) {
                word = new Word(en, pl);
            }
            else {
                word = new Word(en, pl, def);
            }

            word.addCategory( Category.addCategory(cat) );
            if(set instanceof WordList wordList) {
                wordList.addWord(word);
                Core.printLog("dodano nowe słówko: " + word + " do zestawu: " + wordList);
            }
            else {
                // zestaw "Master"
                WordList.getMasterList().addWord(word);
                Core.printLog("dodano nowe słówko: " + word + " do zestawu: " + WordList.getMasterList());
            }


            Core.Instance().getMainWindow().getMainPanel().resetElements();    // zresetowanie fiszek
            this.dispose(); // zamknięcie okna
        }
    }
}
