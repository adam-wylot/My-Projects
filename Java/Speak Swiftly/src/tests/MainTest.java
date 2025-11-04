package tests;

import common.Word;
import common.WordList;

import java.util.ArrayList;

public class MainTest {
    public static final boolean runTests = false;

    // Konstruktor
    private MainTest() {}

    // Testy
    public static void run() {
        if (runTests) {

        // Testy jednostkowe z gałęzi: Adam
            // Test 1
            if (Test1.run()) {
                System.out.println("Test 1 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 1 zakończono niepowodzeniem.");
            }


            if (Test2.run()) {
                System.out.println("Test 2 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 2 zakończono niepowodzeniem.");
            }

            if (Test3.run()) {
                System.out.println("Test 3 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 3 zakończono niepowodzeniem.");
            }

            // Testy jednostkowe z gałęzi: Szymon

            if (Test4.run()) {
                System.out.println("Test 4 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 4 zakończono niepowodzeniem");
            }

            if (Test5.run()) {
                System.out.println("Test 5 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 5 zakończono niepowodzeniem");
            }

            if (Test6.run()) {
                System.out.println("Test 6 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 6 zakończono niepowodzeniem");
            }

            //Testy jednostkowe z gałęzi Jakub

            if (Test7.run()) {
                System.out.println("Test 7 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 7 zakończono niepowodzeniem");
            }

            if (Test8.run()) {
                System.out.println("Test 8 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 8 zakończono niepowodzeniem");
            }

            if (Test9.run()) {
                System.out.println("Test 9 zakończono pomyślnie");
            }
            else {
                System.out.println("Test 9 zakończono niepowodzeniem");
            }

            // Testy jednostkowe metod ze Sprintu 5
            if (Test10.run()) {
                System.out.println("Test 10 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 10 zakończono niepowodzeniem.");
            }

            if (Test11.run()) {
                System.out.println("Test 10 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 10 zakończono niepowodzeniem.");
            }

            if (Test12.run()) {
                System.out.println("Test 10 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 10 zakończono niepowodzeniem.");
            }

            if (Test13.run()) {
                System.out.println("Test 10 zakończono pomyślnie.");
            }
            else {
                System.out.println("Test 10 zakończono niepowodzeniem.");
            }

            System.exit(0);
        }
    }

    public boolean testRemoveWord(WordList wordTest){
        Word word = new Word("ENGTest", "PLTest");
        int tWordID = word.getId();

        wordTest.addWord(word);
        wordTest.removeWord(tWordID);

        for (int i = 0; i < wordTest.getWordsSize(); i++) {
            if (wordTest.getWordByItsId(i).getId() == tWordID) {
                // Słowo nadal znajduje się na liście, więc test nie przeszedł
                return false;
            }
        }

        return true;
    }

    public boolean testRemoveUsageExampleValidIndex(WordList wordTest){
        // ID słowa które już istnieje
        int wordID = 1;
        String usageExample = "Testowy przykład użycia";

        wordTest.addUsageExample(wordID, usageExample);

        // Sprawdzamy, czy dodany przykład użycia jest dostępny w danym słowie
        Word word = wordTest.getWordByItsId(wordID);
        ArrayList<String> usageExamples = word.getUsageExamples();

        boolean isExampleAdded = false;

        for (String example : usageExamples) {
            if (example.equals(usageExample)) {
                isExampleAdded = true;
                break;
            }
        }

        return isExampleAdded;
    }

    public boolean testAlterWordDefinition(WordList wordTest) {
        // ID słowa, które już istnieje
        int wordID = 1;
        String newDefinition = "Nowa definicja";

        // Modyfikujemy definicję słowa o podanym ID
        boolean isDefinitionAltered = wordTest.alterWordDefinition(wordID, newDefinition);

        // Sprawdzamy, czy definicja słowa została zmieniona
        if (isDefinitionAltered) {
            // Pobieramy zmodyfikowane słowo i sprawdzamy, czy jego definicja jest równa nowej definicji
            Word word = wordTest.getWordByItsId(wordID);
            return word.getDef().equals(newDefinition);
        } else {
            // Jeśli funkcja alterWordDefinition zwróciła false, to znaczy, że słowo o podanym ID nie zostało znalezione
            return false;
        }
    }

}
