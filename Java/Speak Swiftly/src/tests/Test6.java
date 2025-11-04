package tests;

import common.Word;
import common.WordList;

public class Test6 {
    // test Word.RemoveWordFromList()
    private Test6(){}

    public static boolean run()
    {
        WordList wordList = new WordList("test");
        Word word = new Word("book", "książka");
        wordList.addWord(word);
        int initialSize = wordList.getWords().size();
        wordList.removeWord(word.getId());
        return wordList.getWords().size() == initialSize - 1;
    }
}
