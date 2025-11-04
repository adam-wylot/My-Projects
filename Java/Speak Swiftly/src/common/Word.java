package common;

import java.io.Serializable;
import java.util.ArrayList;

public class Word implements Serializable {
// Zmienne
    private static int iterator = 0;

    private final int id;
    private String wordEN;
    private String wordPL;
    private String def;
    private ArrayList<Category> categories;
    private ArrayList<String> usageExamples;


// Konstruktory
    public Word(String wordEN, String wordPL) {
        id = ++iterator;
        this.wordEN = wordEN;
        this.wordPL = wordPL;
        def = null;
        categories = new ArrayList<>();
        usageExamples = new ArrayList<>();
    }

    public Word(String wordEN, String wordPL, String def) {
        this(wordEN, wordPL);
        this.def = def;
    }


// Gettery
    public String getWordEN() { return this.wordEN; }
    public String getWordPL() { return this.wordPL; }
    public String getDef() { return this.def; }
    public Category getCategory(int index) { return this.categories.get(index); }
    public ArrayList<Category> getCategories() {
        return this.categories;
    }

    public int getId() { return this.id; }

    public ArrayList<String> getUsageExamples() {
        return this.usageExamples;
    }


// Settery
    public void setWordEN(String wordEN) { this.wordEN = wordEN; }
    public void setWordPL(String wordPL) { this.wordPL = wordPL; }
    public void setDef(String def) { this.def = def; }


// Metody
    public void addCategory(Category category) {
        this.categories.add(category);
    }
    public void removeCategory(Category category) {
        this.categories.remove(category);
    }

    public void addUseExample(String useExample) {
        this.usageExamples.add(useExample);
    }
    public void removeUseExample(int index) {
        this.usageExamples.remove(index);
    }

    @Override
    public String toString() {
        return wordEN + " [" + wordPL + "]";
    }
}
