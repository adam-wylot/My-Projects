package GUI.panels;

import GUI.*;
import GUI.buttons.*;
import common.Core;

import javax.swing.*;
import java.awt.*;

public class MenuPanel extends JPanel {

// Elementy
    private final MenuButton addWordButton;
    private final MenuButton editWordButton;
    private final MenuButton newSetButton;
    private final MenuButton browseSetsButton;
    private final MenuButton testWordButton;


// Konstruktor
    public MenuPanel(int width, int height) {
        setBackground(Core.Instance().getProgramColor().darker());
        setLayout(new GridLayout(16, 1));
        setPreferredSize(new Dimension(width, height));

    // Przyciski
        addWordButton = new MenuButton("Dodaj słówko", e -> new AddWordWindow((int)(800 * Core.Instance().getHorScale()), (int)(300 * Core.Instance().getVerScale())));
        this.add(addWordButton);

        editWordButton = new MenuButton("Edytuj słówko", e -> new EditWordWindow((int)(800 * Core.Instance().getHorScale()), (int)(370 * Core.Instance().getVerScale())));
        this.add(editWordButton);

        // przerwa
        this.add(new JLabel(" "));

        newSetButton = new MenuButton("Utwórz zestaw", e -> new NewSetWindow());
        this.add(newSetButton);

        browseSetsButton = new MenuButton("Przejrzyj zestawy", e -> new SetsBrowserWindow());
        this.add(browseSetsButton);

        // przerwa
        this.add(new JLabel(" "));

        testWordButton = new MenuButton("Rozpocznij test", e -> new TestWordWindow((int) (800 * Core.Instance().getHorScale()), (int) (450 * Core.Instance().getVerScale())));
        this.add(testWordButton);
    }
}
