package common;

import interfaces.ICallBack;
import numerators.Direction;

import java.util.ArrayList;


public class PathFinder extends Thread {
// Zmienne
    private static boolean isTaskActive = false;
    private Maze maze;
    private final ArrayList<Cell> path;
    private boolean foundPath;
    private ICallBack callback;


// Konstruktor
    public PathFinder(Maze maze) {
        callback = null;
        this.maze = maze;
        path = new ArrayList<>();
        foundPath = false;
    }


// Gettery
    public ArrayList<Cell> getPath() { return path; }
    public boolean FoundPath() { return foundPath; }


// Settery
    public void setCallback(ICallBack callback) { this.callback = callback; }


// Metody
    @Override
    public void run() {
        if (isTaskActive) {
            return;
        }
        isTaskActive = true;

        Cell cell;
        CellQueue queue = new CellQueue();
        boolean[][] visited = new boolean[maze.getWidth()][maze.getHeight()];   /// [x][y]

        queue.add(maze.getStartCell());


    // Algorytm
        while (queue.getSize() > 0) {
        // pobranie kolejnej komórki z kolejki
            cell = queue.getCell();
            if (cell == null) {   // zabezpieczenie
                break;
            }
            if (cell == maze.getEndCell()) {
                break;
            }

            visited[cell.getX()][cell.getY()] = true;


        // dodanie nieodwiedzonych "dzieci" do kolejki
            /// GÓRA
            if (cell.isWallOpen(Direction.Up) && !visited[cell.getX()][cell.getY() - 1]) {
                queue.add(maze.getCellAt(cell.getX(), cell.getY() - 1));
            }
            /// PRAWO
            if (cell.isWallOpen(Direction.Right) && !visited[cell.getX() + 1][cell.getY()]) {
                queue.add(maze.getCellAt(cell.getX() + 1, cell.getY()));
            }
            /// DÓŁ
            if (cell.isWallOpen(Direction.Down) && !visited[cell.getX()][cell.getY() + 1]) {
                queue.add(maze.getCellAt(cell.getX(), cell.getY() + 1));
            }
            /// LEWO
            if (cell.isWallOpen(Direction.Left) && !visited[cell.getX() - 1][cell.getY()]) {
                queue.add(maze.getCellAt(cell.getX() - 1, cell.getY()));
            }


        // kolejna iteracja — kolejny obiekt
            queue.next();
        }

    // Sprawdzenie, czy ostatnia komórka badana przez algorytm to wyjście
        cell = queue.getCell();
        if (cell != null)
            setPath(queue);

        // czyszczenie kolejki
        queue.clean();

        isTaskActive = false;
        if (callback != null)
            callback.onThreadComplete();
    }

    private void setPath(CellQueue queue) {
        if (queue.getSize() <= 0)
            return;

        this.foundPath = true;

        Cell cell = queue.getCell();
        path.clear();
        path.add(cell);

        while (queue.goToParent() == 0) {
            cell = queue.getCell();
            path.add(0, cell);
        }

        // ustawienie ścieżki w labiryncie
        maze.setPath(path);
    }
}
