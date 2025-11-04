package tests;

import common.Word;
import common.WordList;

public class Test3 {
    // testuje WordList.removeUsageExample()

    private Test3() {}

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


        // Usunięcie użyć
        wordList.removeUsageExample(word1.getId(), 0);

        wordList.removeUsageExample(word2.getId(), 1);
        wordList.removeUsageExample(word2.getId(), 0);

        wordList.removeUsageExample(word3.getId(), 0);
        wordList.removeUsageExample(word3.getId(), 5);


        // Sprawdzenie poprawności
        if( word1.getUsageExamples().size() != 2 )
            return false;

        if( word2.getUsageExamples().size() != 0 )
            return false;

        if( word3.getUsageExamples().size() != 0 )
            return false;

        return true;
    }
}
