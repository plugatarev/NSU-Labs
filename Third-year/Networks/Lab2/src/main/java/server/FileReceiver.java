package server;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public record FileReceiver(int port, Socket socket) implements Runnable {
    private static final int BUFFER_SIZE = 1024;

    @Override
    public void run() {
        try (DataInputStream input = new DataInputStream(socket.getInputStream())) {
            int filenameLength = receiveFilenameLength(input);
            String filename = receiveFilename(input, filenameLength);
            long size = receiveFileSize(input);
            try (FileOutputStream output = createFileOutputStream(filename)) {
                receiveFile(input, output, size);
            }
        } catch (RuntimeException | IOException e) {
            e.printStackTrace();
            System.out.println("File not transferred");
            return;
        }
        System.out.println("File transferred");
    }

    private FileOutputStream createFileOutputStream(String fileName) throws IOException {
        String separator = System.getProperty("file.separator");
        Path path = Paths.get(Paths.get("").toAbsolutePath() + separator + "uploads" + separator + fileName);
        Files.createFile(path);
        return new FileOutputStream(path.toFile());
    }

    private void receiveFile(DataInputStream input, FileOutputStream output, long size) throws IOException {
        if (size < BUFFER_SIZE) {
            ByteBuffer buffer = readNByte(input, (int)size);
            output.write(buffer.array());
        }
        long iteration = size / BUFFER_SIZE;
        long ostBytes = size % BUFFER_SIZE;
        for (int i = 0; i < iteration; i++) {
            ByteBuffer buffer = readNByte(input, BUFFER_SIZE);
            output.write(buffer.array());
        }
        ByteBuffer buffer = readNByte(input, (int)ostBytes);
        output.write(buffer.array());
    }

    private long receiveFileSize(DataInputStream input) throws IOException {
        return input.readLong();
    }

    private ByteBuffer readNByte(DataInputStream input, int N) throws IOException {
        byte[] buffer = new byte[N];
        int numberRead = 0;
        do {
            int res = input.read(buffer, numberRead, N - numberRead);
            if (res < 1) continue;
            numberRead += res;
        } while (numberRead < N);
        ByteBuffer byteBuffer = ByteBuffer.allocate(N);
        byteBuffer.put(buffer);
        return byteBuffer;
    }

    private String receiveFilename(DataInputStream input, int length) throws IOException {
        String name = new String(readNByte(input, length).array(), StandardCharsets.UTF_8); //TODO: strange
        if (name.contains("/") || name.contains("\\")) throw new FilenameException();
        return name;
    }

    private int receiveFilenameLength(DataInputStream input) throws IOException {
        return input.readInt();
    }
}
