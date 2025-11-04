package common;

import javax.swing.*;
import java.io.*;
import java.util.*;

/* === Lista słówek — inaczej nazywając: zestaw słówek === */
public class WordList {

    private static final ArrayList<WordList> wordLists = new ArrayList<>();

// Zmienne
    private String name;
    private String filePath;
    private List<Word> words;




// Konstruktor
    public WordList(String name, boolean isFileName) {
        this.name = name;
        filePath = Core.wordListsPath + Core.systemSep + name + ".txt";
        words = new ArrayList<>();

        writeFile();
        wordLists.add(this);
    }

    public WordList(String filePath) {
        this.filePath = filePath;
        words = new ArrayList<>();
        try {
            name = filePath.substring(filePath.lastIndexOf(Core.systemSep) + 1, filePath.lastIndexOf('.'));
        }
        catch (Exception e) {
            name = "";
        }

        readFile();
        wordLists.add(this);
    }




// Gettery
    // statyczne
    public static ArrayList<WordList> getAll() { return wordLists; }
    public static int count() { return wordLists.size(); }
    public static WordList get(int index) { return wordLists.get(index); }
    public static WordList getMasterList() { return wordLists.get(0); }
    public static WordList get(String name) {
        for (WordList wordList : wordLists) {
            if (wordList.getName().equals(name)) {
                return wordList;
            }
        }

        return null;
    }

    // normalne
    public String getName() { return name; }
    public List<Word> getWords() { return words; }
    public int getWordsSize() { return words.size(); }
    public Word getWordByItsId(int id) {
        for (Word word : words) {
            if (word.getId() == id) {
                return word;
            }
        }

        // Jeśli słowo o podanym id nie zostało znalezione, zwracamy null
        return null;
    }




// Settery
    public void setWords(List<Word> words) { this.words = words; }




// Metody
    // statyczne
    public static boolean exists(String name) {
        if (count() == 0) {
            Core.printErr("nie istnieje żaden zestaw słówek");
            return false;
        }

        for (WordList wordList : wordLists) {
            if (wordList.name.equalsIgnoreCase(name)) {
                return true;
            }
        }

        return false;
    }

    public static WordList find(String name) {
        if (count() == 0) {
            Core.printErr("nie istnieje żaden zestaw słówek");
            return null;
        }

        for (WordList wordList : wordLists) {
            if (wordList.name.equals(name)) {
                return wordList;

            }
        }
        return null;
    }




    // Dodawanie oraz usuwanie
    public void addWord(Word word) {
        words.add(word);
    }

    public boolean removeWord(int wordID) {
        /// Na przyszłość: najlepiej zrobić tak, że ta funkcje sie wywołuje kiedy klika się na jakiś przycisk "x" przy słowie w GUI, ponieważ wtedy pobiera wordID i odrazu
        /// usuwa :D - to też się aplikuje do dosłownie każdej innej funkcji z tej części!

        for (int i = 0; i < words.size(); i++) {
            if (words.get(i).getId() == wordID) {
                words.remove(i);
                return true;
            }
        }

        return false;
    }

    public boolean addUsageExample(int wordId, String usageExample) {
        for (Word word : words) {
            if (word.getId() == wordId) {
                word.addUseExample(usageExample);
                return true;
            }
        }
        return false;
    }

    public boolean removeUsageExample(int wordId, int exampleIndex) {
        for (Word word : words) {
            if (word.getId() == wordId) {
                if (exampleIndex >= 0 && exampleIndex < word.getUsageExamples().size()) {
                    word.removeUseExample(exampleIndex);
                    return true; // Usunięto przykład użycia pomyślnie
                }
                else {
                    return false; // Nieprawidłowy indeks przykładu użycia
                }
            }
        }
        return false; // Nie znaleziono słowa o podanym identyfikatorze
    }





    /// Zmienianie wartości
    public boolean alterWordDefinition(int wordID, String newDefinition) {
        for( int i = 0; i < words.size(); i++){
            if(words.get(i).getId() == wordID){
                words.get(i).setDef(newDefinition);
                return true;
            }
        }
        return false;
    }

    public boolean alterWordPL(int wordID, String polishTranslation) {

        for( int i = 0; i < words.size(); i++){
            if(words.get(i).getId() == wordID){
                words.get(i).setWordPL(polishTranslation);
                return true;
            }
        }

        return false;
    }

    public boolean alterWordENG(int wordID, String englishTranslation) {

        for( int i = 0; i < words.size(); i++){
            if(words.get(i).getId() == wordID){
                words.get(i).setWordEN(englishTranslation);
                return true;
            }
        }

        return false;
    }





    /// Zapis oraz odczyt z plików
    public static void saveWordLists(String path) {
        for (WordList wordList : wordLists) {
            wordList.writeFile();
        }
    }

    public static void loadWordLists(String path) {
        // obiekt File reprezentujący katalog o ścieżce path
        File directory = new File(path);

        if (!directory.exists()) {
            if (directory.mkdirs() == false) {
                Core.printErr("nie udało się utworzyć folderu w lokalizacji: " + directory.getAbsolutePath());
                JOptionPane.showMessageDialog(null, "Nie udało się utworzyć wymaganego folderu! (Sprawdź log)\nUwaga: funkcja zapisu słówek na dysku jest niedostępna", "Błąd krytyczny", JOptionPane.ERROR_MESSAGE);
                return;
            }
        }

        if (directory.isDirectory()) {
            File[] files = directory.listFiles((dir, name) -> name.toLowerCase().endsWith(".txt")); // pobranie wszystkich plików z katalogu

            if (files != null) {
                String masterPath = path + Core.systemSep + Core.masterWLname + ".txt";

                if (new File(masterPath).exists()) {
                    // jako pierwsze wczytanie głównej listy, żeby była na 1. indeksie
                    new WordList(masterPath);
                }
                else {
                    // stworzenie nowego pliku master
                    new WordList(Core.masterWLname, true);
                }


                for (File file : files) {
                    if (file.getName().equals(Core.masterWLname + ".txt"))
                        continue;

                    new WordList(file.getPath());   // wczytanie listy słów
                }
            }
            else {
                // stworzenie nowego pliku master
                new WordList(Core.masterWLname, true);
            }
        }
    }

    public void writeFile() {
        try (ObjectOutputStream outputStream = new ObjectOutputStream(new FileOutputStream(filePath))) {
            outputStream.writeObject(words);
            Core.printLog("pomyślnie zapisano dane do pliku: " + filePath);
        }
        catch (IOException e) {
            Core.printErr("nie udało się zapisać danych do pliku: " + e.getMessage());
        }
    }

    @SuppressWarnings("unchecked")
    private void readFile() {
        try (ObjectInputStream inputStream = new ObjectInputStream(new FileInputStream(filePath))) {
            words = (List<Word>) inputStream.readObject();
            Core.printLog("pomyślnie wczytano dane z pliku: " + filePath);
        }
        catch (IOException | ClassNotFoundException e) {
            Core.printErr("nie udało się wczytać danych z pliku: " + e.getMessage());
        }
    }

    public void deleteFile() {
        File file = new File(filePath);

        if (file.exists()) {
            if (file.delete()) {
                Core.printLog("plik został pomyślnie usunięty (" + filePath + ")");
            }
            else {
                Core.printErr("nie udało się usunąć pliku (" + filePath + ")");
            }
        } else {
            Core.printErr("plik nie istnieje (" + filePath + ")");
        }
    }





    /// Inne
    public List<Word> filterWordsByCategory(String category) {
        List<Word> filteredWords = new ArrayList<>();
        for (Word word : words) {
            for (Category cat : word.getCategories()) {
                if (cat.getCategoryName().equals(category)) {
                    filteredWords.add(word);
                    break; // Przerwij pętlę, gdy słowo zostało dodane
                }
            }
        }

        return filteredWords;
    }

//    public List<Word> filterWordsByCategory(String categoryName) {
//        return words.stream()
//                .filter(word -> word.getCategories().stream()
//                        .anyMatch(category -> category.getCategoryName().equals(categoryName)))
//                .collect(Collectors.toList());
//    }


    public void rename(String newName) {
        if (exists(newName)) {
            JOptionPane.showMessageDialog(null, "Zestaw o takiej nazwie już istnieje!", "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }

        if (name.equalsIgnoreCase("master")) {
            JOptionPane.showMessageDialog(null, "Nie można ustawić nazwy zestawu na nazwę zestawu głównego!", "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }

        // Zmiana nazwy
        name = newName;
        filePath = Core.wordListsPath + Core.systemSep + newName + ".txt";
    }

    @Override
    public String toString() {
        return name;
    }
}