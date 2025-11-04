package common;

import java.io.*;
import java.util.ArrayList;

public class Category implements Serializable {
    // Zmienne
    private static int counter;
    private static ArrayList<Category> categories;

    private int id;
    private String categoryName;

    // Konstruktory
    static { /// blok inicjalizacyjny statyczny
        counter = 0;
        categories = new ArrayList<>();
    }

    public Category(String categoryName) {
        id = ++counter;
        this.categoryName = categoryName;
        categories.add(this);
    }

    // Gettery
    public static int instancesCount() {
        return counter;
    }

    public int getId() {
        return this.id;
    }

    public String getCategoryName() {
        return this.categoryName;
    }

    public static Category getCategory(String categoryName) {
        for (Category c : categories) {
            if (c.categoryName.equals(categoryName)) {
                return c;
            }
        }

        return null;
    }

    // Metody
    private static boolean exists(String name) {
        for (Category c : categories) {
            if (c.categoryName.equals(name)) {
                return true;
            }
        }

        return false;
    }

    public static Category addCategory(String name) {
        Category c = Category.getCategory(name);
        if (c == null) {
            c = new Category(name);
        }

        return c;
    }
}
