package GUI.panels;

import common.Core;

import javax.swing.*;
import java.awt.*;

public class ScrollPanel extends JScrollPane {
// Zmienne
    private UtilityPanel utilityPanel;


// Konstruktor
    public ScrollPanel(Component view) {
        super(view);

        JPanel backgroundPanel = new JPanel(new BorderLayout());

        // Dodanie panelu obok poziomego paska przewijania
        utilityPanel = new UtilityPanel((int)(Core.ScaleX() * 400), (int)(Core.ScaleY() * 23));
        backgroundPanel.add(utilityPanel, BorderLayout.EAST);

        // Zastosowanie niestandardowego układu do JScrollPane
        setLayout(new ScrollPanelLayout(backgroundPanel));

        setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        getVerticalScrollBar().setUnitIncrement(40);
        getHorizontalScrollBar().setUnitIncrement(40);
    }

    private static class ScrollPanelLayout extends ScrollPaneLayout {
        private final Component additionalComponent;

        public ScrollPanelLayout(Component additionalComponent) {
            this.additionalComponent = additionalComponent;
        }

        @Override
        public void layoutContainer(Container parent) {
            super.layoutContainer(parent);

            if (hsb != null && hsb.isVisible()) {
                // Ustawienia paska przewijania
                Rectangle hsbBounds = hsb.getBounds();
                hsbBounds.width -= (int)(Core.ScaleX() * 400);
                hsb.setBounds(hsbBounds);

                // Ustawienia dodatkowego panelu obok paska przewijania
                Rectangle additionalBounds = new Rectangle(hsbBounds);
                additionalBounds.x += (int)(Core.ScaleX() * 400);
                additionalBounds.width = hsbBounds.width;
                additionalComponent.setBounds(additionalBounds);

                parent.add(additionalComponent);
            }
        }
    }

    // Gettery
    public UtilityPanel getUtilityPanel() { return utilityPanel; }
}
