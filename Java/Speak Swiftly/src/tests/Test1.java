package tests;

import common.Category;
import common.Word;
import common.WordList;

import java.util.List;

public class Test1 {
    // testuje WordList.filterWordsByCategory()

    private Test1() {}

    public static boolean run() {
        Word word;
        WordList wordList = new WordList("test");
        List<Word> filteredlist;

        // utworzenie kategorii
        Category.addCategory("owoce");
        Category.addCategory("transport");
        Category.addCategory("meble");
        Category.addCategory("kuchnia");

        // dodanie słówek
        word = new Word("apple", "jabłko");
        word.addCategory(Category.getCategory("owoce"));
        wordList.addWord(word);

        word = new Word("orange", "pomarańcza");
        word.addCategory(Category.getCategory("owoce"));
        wordList.addWord(word);

        word = new Word("banana", "banan");
        word.addCategory(Category.getCategory("owoce"));
        wordList.addWord(word);

        word = new Word("pear", "gruszka");
        word.addCategory(Category.getCategory("owoce"));
        wordList.addWord(word);


        word = new Word("car", "samochód");
        word.addCategory(Category.getCategory("transport"));
        wordList.addWord(word);

        word = new Word("train", "pociąg");
        word.addCategory(Category.getCategory("transport"));
        wordList.addWord(word);


        word = new Word("table", "stół");
        word.addCategory(Category.getCategory("meble"));
        wordList.addWord(word);


        // sprawdzanie poprawności funkcji
        filteredlist = wordList.filterWordsByCategory("owoce");
        if (filteredlist.size() != 4) { return false; }

        filteredlist = wordList.filterWordsByCategory("transport");
        if (filteredlist.size() != 2) { return false; }

        filteredlist = wordList.filterWordsByCategory("meble");
        if (filteredlist.size() != 1) { return false; }

        filteredlist = wordList.filterWordsByCategory("kuchnia");
        if (!filteredlist.isEmpty()) { return false; }

        filteredlist = wordList.filterWordsByCategory("123");
        if (!filteredlist.isEmpty()) { return false; }

        return true;
    }
}
