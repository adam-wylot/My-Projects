package exceptions;

public class UnknownFileTypeException extends Exception {
    public UnknownFileTypeException() {
        super();
    }

    public UnknownFileTypeException(String message) {
        super(message);
    }
}
