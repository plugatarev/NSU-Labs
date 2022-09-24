package client;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public final class FileSender {
    private static final int BUFFER_SIZE = 1024;
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
             FileInputStream fileStream = new FileInputStream(file)) {
            sendFilenameLength(socketWriter);
            sendFilename(socketWriter);
            sendFileSize(file, socketWriter);
            byte[] buffer = new byte[BUFFER_SIZE];
            int count;
            while ((count = fileStream.read(buffer)) > -1) {
                socketWriter.write(buffer, 0, count);
            }
        } catch (IOException e) {
            e.printStackTrace();
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
