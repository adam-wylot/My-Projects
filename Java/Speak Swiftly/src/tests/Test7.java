package tests;

import common.Word;
import common.WordList;

public class Test7 {
    // test WordList.alterWordENG()
    private Test7(){}

    public static boolean run()
    {
        WordList wordList = new WordList("unitTest7");
        Word word1 = new Word("irony", "żelazo");
        Word word2 = new Word("heir", "włosy");
        wordList.addWord(word1);
        wordList.addWord(word2);

        wordList.alterWordENG(word1.getId(), "iron");
        wordList.alterWordENG(word2.getId(), "hair");

        if (word1.getWordEN().equals("iron")) return false;
        if (word2.getWordEN().equals("hair")) return false;
        return true;
    }
}
