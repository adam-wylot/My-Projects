package common;

// klasa pomocnicza
class CellQueueElem {
    Cell cell;
    CellQueueElem parent;
    CellQueueElem next;

    CellQueueElem(Cell cell, CellQueueElem parent) {
        this.cell = cell;
        this.parent = parent;
    }
}


// === Klasa publiczna ===
public class CellQueue {
    // Zmienne
    private CellQueueElem head;
    private CellQueueElem last;
    private int size;


    // Konstruktory
    public CellQueue() {
        head = null;
        last = null;
        size = 0;
    }


    // Gettery
    public Cell getCell() {
        if (head == null)
            return null;

        return head.cell;
    }

    public int getSize() { return size; }


    // Metody
    public void add(Cell cell) {
        CellQueueElem elem;

        if (last == null) {
            elem = new CellQueueElem(cell, null);
            head = elem;
        }
        else {
            elem = new CellQueueElem(cell, head);
            last.next = elem;
        }

        last = elem;
        size++;
    }

    public void next() {
        if (head == null)
            return;

        if (head.next == null) {
            head = null;
            last = null;
            size = 0;
            return;
        }

        head = head.next;
        size--;
    }

    public void clean() {
        size = 0;
        head = null;
        last = null;
    }

    public int goToParent() {
        if (head.parent != null) {
            head = head.parent;
            return 0;
        }

        return 1;
    }
}