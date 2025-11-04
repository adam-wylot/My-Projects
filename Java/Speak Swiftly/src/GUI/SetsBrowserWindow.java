package GUI;

import common.Core;
import common.SetsBrowser;
import common.Word;
import common.WordList;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class SetsBrowserWindow extends JDialog {

// Zmienne
    private SetsBrowser setsBrowser;
    private boolean wordMode;
    private boolean isChanged;


// Elementy
    private final JTextField setNameTF;

    private final JList<Object> list;
    private final DefaultListModel<Object> listModel;

    private final JPanel buttonPanel;
    private final JButton backButton;
    private final JButton deleteButton;
    private final JButton resetButton;
    private final JButton saveButton;


// Konstruktory
    public SetsBrowserWindow(int width, int height) {
        super(Core.Instance().getMainWindow(), "Dostępne zestawy", Dialog.ModalityType.APPLICATION_MODAL);

    // zmienne
        setsBrowser = new SetsBrowser();
        wordMode = false;
        isChanged = false;


    // inicjalizacja okna
        setSize(width, height);
        setLayout(new BorderLayout());
        setResizable(false);   // brak możliwości zmiany wielkości okna
        setLocationRelativeTo(null);   // wycentrowanie okna
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        setBackground(Core.Instance().getProgramColor());


    // Elementy okna
        // Text Field z nazwą zestawu
        setNameTF = new JTextField();
        setNameTF.setVisible(false);
        this.add(setNameTF, BorderLayout.NORTH);


        // lista z obiektami
        listModel = new DefaultListModel<>();
        list = new JList<>(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        list.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    int index = list.locationToIndex(e.getPoint());
                    if (index >= listModel.getSize()) {
                        return;
                    }

                    Object selectedItem = listModel.getElementAt(index);

                    if (selectedItem instanceof WordList) {
                        setsBrowser.setActiveSet((WordList) selectedItem);
                        switchMode();
                    }
                    else if (selectedItem instanceof Word) {
                        isChanged = true;
                        saveButton.setEnabled(true);

                        listModel.removeElement(selectedItem);
                    }
                }
            }
        });
        this.add(new JScrollPane(list), BorderLayout.CENTER);
        fillListWithSets();




    // Przyciski
        buttonPanel = new JPanel(new GridLayout(1, 0));
        buttonPanel.setOpaque(false);
        buttonPanel.setVisible(false);
        this.add(buttonPanel, BorderLayout.SOUTH);

        // przycisk: Powrót do zestawów
        backButton = new JButton("Powrót do zestawów");
        backButton.addActionListener(e -> {
            if (isChanged || !setNameTF.getText().equals(setsBrowser.getActiveSet().getName())) {
                if (JOptionPane.showConfirmDialog(null, "Niezapisane zmiany zostaną utracone.\nCzy na pewno chcesz wrócić do poprzedniego widoku?") != 0) {
                    return;
                }
            }

            switchMode();
        });
        buttonPanel.add(backButton);

        // przycisk: Usuń zestaw
        deleteButton = new JButton("Usuń zestaw");
        deleteButton.addActionListener(e -> {
            if (setsBrowser.getActiveSet() != null) {
                if (JOptionPane.showConfirmDialog(null, "Czy na pewno chcesz usunąć ten zestaw?") == 0) {
                    setsBrowser.getSets().remove(setsBrowser.getActiveSet());
                    setsBrowser.getActiveSet().deleteFile();

                    Core.Instance().getMainWindow().getMainPanel().resetElements();    // zresetowanie fiszek
                    Core.printLog("usunięto zestaw: " + setsBrowser.getActiveSet().getName());
                    switchMode();
                }
            }
            else {
                JOptionPane.showMessageDialog(null, "Nie wybrano zestawu!", "Błąd", JOptionPane.ERROR_MESSAGE);
            }
        });
        buttonPanel.add(deleteButton);

        // przycisk: Reset
        resetButton = new JButton("Resetuj zmiany");
        resetButton.addActionListener(e ->  {
            fillListWithWords();
            setNameTF.setText(setsBrowser.getActiveSet().getName());
            isChanged = false;
        });
        buttonPanel.add(resetButton);

        // przycisk: Zapisz zmiany
        saveButton = new JButton("Zapisz zmiany");
        saveButton.addActionListener(e -> saveChanges());
        buttonPanel.add(saveButton);


        this.setVisible(true);
    }

    public SetsBrowserWindow() {
        this((int)(Core.Instance().getHorScale() * 800), (int)(Core.Instance().getVerScale() * 600));
    }


// Metody
    private void fillListWithSets() {
        listModel.clear();

        for (WordList wordList : setsBrowser.getSets()) {
            listModel.addElement(wordList);
        }
    }

    private void fillListWithWords() {
        listModel.clear();

        for (Word word : setsBrowser.getSetWords()) {
            listModel.addElement(word);
        }
    }

    private void switchMode() {
        wordMode = !wordMode;

        if (wordMode) {
            fillListWithWords();
            setNameTF.setText(setsBrowser.getActiveSet().getName());
            setNameTF.setEnabled(true);
            deleteButton.setEnabled(true);
            if (setsBrowser.getActiveSet().getName().equalsIgnoreCase( Core.masterWLname )) {
                // oddzielne ustawiena dla głównego zestawu "Master"
                setNameTF.setEnabled(false);
                deleteButton.setEnabled(false);
            }

            buttonPanel.setVisible(true);
        }
        else {
            fillListWithSets();
            isChanged = false;
            setsBrowser.setActiveSet(null);
            buttonPanel.setVisible(false);
        }
    }

    private void saveChanges() {
        // zapisanie nazwy zestawu, jeżeli się zmieniła
        if (wordMode && !setsBrowser.getActiveSet().getName().equals(setNameTF.getText())) {
            setsBrowser.getActiveSet().rename(setNameTF.getText());
        }

        if (isChanged) {
            setsBrowser.changeWords(listModel);
            isChanged = false;
        }
        fillListWithWords();
        new Thread(() -> setsBrowser.getActiveSet().writeFile()).start();    // zapis do pliku


        Core.Instance().getMainWindow().getMainPanel().resetElements();    // zresetowanie fiszek

        // powiadomienia
        JOptionPane.showMessageDialog(null, "Zmiany zostały zapisane.", Core.programName, JOptionPane.INFORMATION_MESSAGE);
        Core.printLog("Zapisano zmiany wprowadzone do zestawów słówek");
    }
}
