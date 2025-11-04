package GUI.buttons;

import common.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

public class MenuButton extends JButton {

// Konstruktor
    public MenuButton(String text, ActionListener actionListener) {
        super();

        setText(text);
        setFocusable(false);
        setFont(Core.Instance().getProgramFont().deriveFont(Font.BOLD, (float) Math.min(Core.Instance().getHorScale(), Core.Instance().getVerScale()) * 32f));
        setBackground(Core.Instance().getProgramColor().darker());
        setBorder(BorderFactory.createEtchedBorder());
        setForeground(Color.WHITE);
        addActionListener(actionListener);
    }
}
