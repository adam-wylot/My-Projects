package interfaces;

import common.Cell;
import exceptions.NoSuchCellException;

public interface IFileHandler {
    int getWidth();
    int getHeight();
    int getStartCellX() throws NoSuchCellException;
    int getStartCellY() throws NoSuchCellException;
    int getEndCellX() throws NoSuchCellException;
    int getEndCellY() throws NoSuchCellException;
    Cell getCellAt(int x, int y) throws NoSuchCellException;
}
