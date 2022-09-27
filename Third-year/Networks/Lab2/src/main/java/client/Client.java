package client;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {
    public static void main(String[] args) {
        String filename = args[0];
        InetAddress serverAddress;
        try {
            serverAddress = InetAddress.getByName(args[1]);
        } catch (UnknownHostException e) {
            System.err.println("Invalid address in arguments");
            return;
        }
        int port = Integer.parseInt(args[2]);
        try (Socket socket = new Socket()) {
            socket.setSoTimeout(20000);
            socket.connect(new InetSocketAddress(serverAddress, port));
            FileSender fileSender = new FileSender(socket, filename);
            fileSender.sendFile();
        } catch (FileNotFoundException e) {
            System.err.println("File " + filename + " not found");
        } catch (IOException e) {
            System.err.println("Couldn't connect to server, because of exception: " + e.getMessage());
        }
    }

}
