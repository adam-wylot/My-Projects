package tests;

import common.Category;
import common.Word;

import java.util.ArrayList;

public class Test5 {
    // test Word.removeCategory()
    private Test5(){}

    public static boolean run()
    {
        Word word = new Word("book", "książka");
        Category category = new Category("owoce");
        word.addCategory(category);
        word.removeCategory(category);
        ArrayList<Category> categories = word.getCategories();
        if (categories.isEmpty()) {
            return true;
        } else {
            return false;
        }
    }
}
