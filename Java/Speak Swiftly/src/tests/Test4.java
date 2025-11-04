package tests;

import common.Category;
import common.Word;
import java.util.ArrayList;

public class Test4 {
    // test Word.addCategory()
    private Test4(){}

    public static boolean run()
    {
        Word word = new Word("book", "książka");
        Category category = new Category("owoce");
        word.addCategory(category);
        ArrayList<Category> categories = word.getCategories();
        if (categories.size() == 1 && categories.get(0).getCategoryName().equals("Noun")) {
            return true;
        } else {
            return false;
        }
    }
}
