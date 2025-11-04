package GUI;

import common.Core;
import common.WordList;

import javax.swing.*;
import java.awt.*;

public class NewSetWindow extends JDialog {

// Elementy
    private final JTextField textField;
    private final JButton createButton;


// Konstruktory
    public NewSetWindow(int width, int height) {
        super(Core.Instance().getMainWindow(), "Utwórz zestaw", Dialog.ModalityType.APPLICATION_MODAL);

        setSize(width, height);
        setLayout(new GridLayout(5, 1));
        setResizable(false);   // brak możliwości zmiany wielkości okna
        setLocationRelativeTo(null);   // wycentrowanie okna
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        setBackground(Core.Instance().getProgramColor());


    // Utworzenie elementów okna
        JLabel nameLabel = new JLabel("Nazwa zestawu:");
        this.add(nameLabel);

        textField = new JTextField();
        this.add(textField);

        this.add(new JLabel(" "));
        this.add(new JLabel(" "));

        createButton = new JButton("Utwórz");
        createButton.addActionListener(e -> button_click());
        this.add(createButton);


        setVisible(true);
    }

    public NewSetWindow() {
        this((int)(Core.Instance().getHorScale() * 320), (int)(Core.Instance().getVerScale() * 180));
    }


// Metody
    private void button_click() {
        String str = textField.getText();

        // obsługa błędów
        if (str.isBlank()) {
            Toolkit.getDefaultToolkit().beep();
            JOptionPane.showMessageDialog(null, "Nazwa zestawu nie może być pusta!", "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (WordList.exists(str)) {
            Toolkit.getDefaultToolkit().beep();
            JOptionPane.showMessageDialog(null, "Zestaw o podanej nazwie już istnieje!", "Błąd", JOptionPane.ERROR_MESSAGE);
            return;
        }

        // dodanie zestawu
        new WordList(str, true);
        Core.printLog("dodano nowy zestaw: " + str);

        this.dispose(); // zamknięcie okna
    }
}
