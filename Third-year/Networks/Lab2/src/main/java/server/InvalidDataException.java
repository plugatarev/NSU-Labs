package server;

import java.io.IOException;

public class InvalidDataException extends IOException {
    public InvalidDataException(String msg) {
        super(msg);
    }
}
