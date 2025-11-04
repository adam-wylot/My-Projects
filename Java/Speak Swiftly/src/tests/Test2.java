package tests;

import common.Category;
import common.Word;
import common.WordList;

import java.util.List;

public class Test2 {
    // testuje WordList.addUsageExample()

    private Test2() {}

    public static boolean run() {
        Word word;
        WordList wordList = new WordList("test");

        // dodanie słówek
        Word word1  = new Word("apple", "jabłko");
        Word word2 = new Word("orange", "pomarańcza");
        Word word3 = new Word("banana", "banan");

        wordList.addWord(word1);
        wordList.addWord(word2);
        wordList.addWord(word3);


        // dodanie przykładów użyć
        wordList.addUsageExample(word1.getId(), "1");
        wordList.addUsageExample(word1.getId(), "2");
        wordList.addUsageExample(word1.getId(), "3");

        wordList.addUsageExample(word2.getId(), "1");
        wordList.addUsageExample(word2.getId(), "2");


        // Sprawdzenie poprawności
        if( word1.getUsageExamples().size() != 3 )
            return false;

        if( word2.getUsageExamples().size() != 2 )
            return false;

        if( word3.getUsageExamples().size() != 0 )
            return false;

        return true;
    }
}
