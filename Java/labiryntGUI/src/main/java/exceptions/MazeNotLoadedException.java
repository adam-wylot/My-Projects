package exceptions;

public class MazeNotLoadedException extends Exception {
    public MazeNotLoadedException() {
        super();
    }

    public MazeNotLoadedException(String message) {
        super(message);
    }
}
