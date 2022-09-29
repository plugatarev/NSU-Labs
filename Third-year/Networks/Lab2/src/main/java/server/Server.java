package server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class Server {
    private static final int BLOCK_TIME = 3000;

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("One argument required: <port>");
            return;
        }
        int port = Integer.parseInt(args[0]);

        Path uploads = Paths.get("uploads");
        if (!Files.isDirectory(uploads)) {
            try {
                Files.createDirectory(uploads);
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
        }
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            while (true) {
                Socket socket = serverSocket.accept();
                socket.setSoTimeout(BLOCK_TIME);
                Thread newClient = new Thread(new FileReceiver(socket));
                newClient.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
