package tests;


import common.Flashcards;
import common.Word;
import common.WordList;

public class Test10 {
    // test Flashcards.getFront()
    private Test10(){}

    public static boolean run()
    {



        WordList wordList = new WordList("TestList", false);
        wordList.addWord(new Word("cat", "kot"));
        wordList.addWord(new Word("dog", "pies"));
        wordList.addWord(new Word("apple", "jabłko"));
        wordList.addWord(new Word("banana", "banan"));

        Flashcards flashcards = new Flashcards();
        flashcards.reset();
        String front = flashcards.getFront();


        return front.equals("cat") || front.equals("dog") || front.equals("apple") || front.equals("banana");
            }


        }



