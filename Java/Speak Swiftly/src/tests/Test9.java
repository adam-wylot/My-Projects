package tests;

import common.Category;
import common.Word;

public class Test9 {
    // test Word.getCategory z usunięciem słowa
    private Test9(){}

    public static boolean run()
    {
        Category categoryA = new Category("kuchnia");
        Category categoryB = new Category("warzywa");
        Category categoryC = new Category("podstawowe");

        Word word = new Word("carrot", "marchewka");
        word.addCategory(categoryA);
        word.addCategory(categoryB);
        word.addCategory(categoryC);

        if (word.getCategory(1) != categoryB) return false;
        word.removeCategory(categoryB);
        if (word.getCategory(1) == categoryB) return false;
        return true;
    }
}
