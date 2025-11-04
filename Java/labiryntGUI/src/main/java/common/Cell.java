package common;

import numerators.Direction;

public class Cell {
    // Zmienne
    private final int x;
    private final int y;
    private byte walls;     /// 1 - ściana otwarta, 0 - ściana zamknięta


    // Konstruktor
    public Cell (int x, int y) {
        this.x = x;
        this.y = y;
        walls = 0x0;
    }


    // Settery
    public void setPass(boolean is_open, Direction direction) {
        if (is_open)
            walls = (byte)( walls | (1 << direction.ordinal()) );
        else
            walls = (byte)( walls & ~(1 << direction.ordinal()) );
    }


    // Gettery
    public int getX() { return x; }
    public int getY() { return y; }

    public boolean isWallOpen(Direction direction) {
        if (((walls >> direction.ordinal()) & 1) == 1) {
            return true;
        }

        return false;
    }


    // Metody
}
