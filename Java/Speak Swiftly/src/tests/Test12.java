package tests;


import common.Flashcards;
import common.Word;
import common.WordList;

public class Test12 {
    // test Flashcards.reset()
    private Test12(){}

    public static boolean run()
    {



        WordList wordList = new WordList("TestList", false);
        wordList.addWord(new Word("cat", "kot"));
        wordList.addWord(new Word("dog", "pies"));
        wordList.addWord(new Word("apple", "jabłko"));
        wordList.addWord(new Word("banana", "banan"));

        Flashcards flashcards = new Flashcards();
        flashcards.reset();
        flashcards.next();


        return flashcards.getWordNumber() == 1;

    }


}



