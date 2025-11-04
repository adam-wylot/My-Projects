package tests;

import common.Word;
import common.WordList;

public class Test8 {
    // test WordList.alterWordPL()
    private Test8(){}

    public static boolean run()
    {
        WordList wordList = new WordList("unitTest8");
        Word word1 = new Word("cows", "krowa");
        Word word2 = new Word("whole", "dziura");
        wordList.addWord(word1);
        wordList.addWord(word2);

        wordList.alterWordPL(word1.getId(), "krowy");
        wordList.alterWordPL(word2.getId(), "całe");

        if (word1.getWordPL().equals("krowy")) return false;
        if (word2.getWordPL().equals("całe")) return false;
        return true;
    }
}
