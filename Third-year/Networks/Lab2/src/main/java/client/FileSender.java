package client;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public final class FileSender {
    private static final int BUFFER_SIZE = 512;
    private final Socket socket;
    private final String filename;
    private final File file;

    public FileSender(Socket socket, String filename) throws FileNotFoundException {
        this.socket = socket;
        file = new File(filename);
        if (!file.exists()) throw new FileNotFoundException("File with name: " + filename + " not found");
        this.filename = file.getName();
    }

    void sendFile() {
        try (DataOutputStream socketWriter = new DataOutputStream(socket.getOutputStream());
             FileInputStream fileStream = new FileInputStream(file);
             DataInputStream input = new DataInputStream(socket.getInputStream())) {
            sendFilenameLength(socketWriter);
            sendFilename(socketWriter);
            sendFileSize(file, socketWriter);
            byte[] buffer = new byte[BUFFER_SIZE];
            int count;
            while ((count = fileStream.read(buffer)) > -1) {
                Thread.sleep(100);
                socketWriter.write(buffer, 0, count);
            }
            boolean flag = input.readBoolean();
            if (flag) System.out.println("File transferred");
            else System.out.println("File not transferred");
        } catch (IOException | InterruptedException e) {
            System.err.println("Client was interrupted with an exception: " + e.getMessage());
        }
    }

    private void sendFilenameLength(OutputStream output) throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(Integer.BYTES);
        buffer.putInt(filename.getBytes(StandardCharsets.UTF_8).length);
        output.write(buffer.array());
    }

    private void sendFilename(OutputStream output) throws IOException {
        output.write(filename.getBytes(StandardCharsets.UTF_8));
    }

    private void sendFileSize(File file, OutputStream output) throws IOException {
        ByteBuffer byteBuffer = ByteBuffer.allocate(Long.BYTES);
        byteBuffer.putLong(file.length());
        output.write(byteBuffer.array());
    }
}
