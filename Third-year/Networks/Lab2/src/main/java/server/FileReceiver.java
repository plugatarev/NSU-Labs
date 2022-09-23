package server;

import java.net.Socket;

public record ReceiveFile(int port, Socket socket) implements Runnable {

    @Override
    public void run() {

    }
}
