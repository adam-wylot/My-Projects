package exceptions;

public class NoSuchCellException extends Exception {
    public NoSuchCellException() {
        super();
    }

    public NoSuchCellException(String message) {
        super(message);
    }
}
