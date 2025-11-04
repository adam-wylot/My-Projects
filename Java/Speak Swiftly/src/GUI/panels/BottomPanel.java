package GUI.panels;

import common.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BottomPanel extends JPanel implements ActionListener {

    private JButton prevButton;
    private JButton nextButton;

    public BottomPanel(int width, int height) {
        super();

        setLayout(new FlowLayout(FlowLayout.CENTER, (int)(Core.Instance().getHorScale() * 5), (int)(Core.Instance().getVerScale() * 5)));
        setPreferredSize(new Dimension(width, height));
        setOpaque(false);


        Dimension buttonDim = new Dimension((int)(Core.Instance().getHorScale() * 350), (int)(Core.Instance().getVerScale() * 80));

        prevButton = new JButton("Poprzednie");
        prevButton.setPreferredSize(buttonDim);
        prevButton.setFocusable(false);
        prevButton.setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 24f));
        prevButton.setBackground(Core.Instance().getProgramColor().darker());
        prevButton.setBorder(BorderFactory.createEtchedBorder());
        prevButton.setForeground(Color.WHITE);
        prevButton.addActionListener(this);
        this.add(prevButton);

        nextButton = new JButton("Następne");
        nextButton.setPreferredSize(buttonDim);
        nextButton.setFocusable(false);
        nextButton.setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 24f));
        nextButton.setBackground(Core.Instance().getProgramColor().darker());
        nextButton.setBorder(BorderFactory.createEtchedBorder());
        nextButton.setForeground(Color.WHITE);
        nextButton.addActionListener(this);
        this.add(nextButton);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == prevButton) {
            Core.Instance().getFlashcards().back();
        }
        else if (e.getSource() == nextButton) {
            Core.Instance().getFlashcards().next();
        }


        if (this.getParent() != null && this.getParent() instanceof MainPanel mainPanel) {
            mainPanel.getFlashcardButton().setReversed(false);
            mainPanel.getFlashcardButton().setFlashcard();

            mainPanel.getTopPanel().setCounter();
        }
    }
}
