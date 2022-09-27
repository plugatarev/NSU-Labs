package server;

import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Timer;
import java.util.TimerTask;

public final class FileReceiver implements Runnable {
    private static final int BUFFER_SIZE = 512;
    private static final int INTERVAL = 3000;
    private static final int KILOBYTE = 1024;
    private final Socket socket;
    private long countBytesInterval;
    private long countBytesTotal;
    private long time;


    public FileReceiver(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        long fileSize;
        long receivedFileSize;
        try (DataInputStream input = new DataInputStream(socket.getInputStream());
             DataOutputStream output = new DataOutputStream(socket.getOutputStream())) {
            int filenameLength = receiveFilenameLength(input);
            String filename = receiveFilename(input, filenameLength);
            fileSize = receiveFileSize(input);
            try (FileOutputStream fileOutputStream = createFileOutputStream(filename)) {
                receivedFileSize = receiveFile(input, fileOutputStream, fileSize);
            }
            boolean isCorrect = checkSize(fileSize, receivedFileSize);
            output.writeBoolean(isCorrect);
        } catch (RuntimeException | IOException e) {
            System.err.println("Thread on port: " + socket.getPort() + " was interrupted with an exception: " + e.getMessage());
        }
    }

    private boolean checkSize(long fileSize, long receivedFileSize) {
        return fileSize == receivedFileSize && fileSize == countBytesTotal;
    }

    private FileOutputStream createFileOutputStream(String fileName) throws IOException {
        String separator = System.getProperty("file.separator");
        Path path = Paths.get(Paths.get("").toAbsolutePath() + separator + "uploads" + separator + fileName);
        Files.createFile(path);
        return new FileOutputStream(path.toFile());
    }

    private long receiveFile(DataInputStream input, FileOutputStream output, long size) throws IOException {
        int count;
        byte[] buffer = new byte[BUFFER_SIZE];
        Timer timer = new Timer();
        setSpeedMeter(timer);
        long start = System.currentTimeMillis();
        while (countBytesTotal < size && (count = input.read(buffer)) > -1) {
            countBytesInterval += count;
            countBytesTotal += count;
            output.write(buffer, 0, count);
        }
        System.out.printf("Thread on port: " + socket.getPort() + "Speed for the entire transmission period: %.2f KB/s\n",
                (double)(countBytesTotal * 1000) / (KILOBYTE * (System.currentTimeMillis() - start)));
        timer.cancel();
        return output.getChannel().size();
    }

    private long receiveFileSize(DataInputStream input) throws IOException {
        return input.readLong();
    }

    private void setSpeedMeter(Timer timer) {
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                time += 3000;
                double speedInterval = (double)(countBytesInterval * 1000) / INTERVAL;
                double speedIntervalTotal = (double)(countBytesTotal * 1000) / time;
                System.out.printf("port=" + socket.getPort() + " Current speed: %.2f KB/s\n", speedInterval / KILOBYTE);
                System.out.printf("port=" + socket.getPort() + " Total speed: %.2f KB/s\n", speedIntervalTotal / KILOBYTE);
                countBytesInterval = 0;
            }
        }, INTERVAL, INTERVAL);
    }

    private String receiveFilename(DataInputStream input, int length) throws IOException {
        byte[] buffer = new byte[length];
        if (input.read(buffer, 0, length) != length) {
            throw new InvalidDataException();
        }
        return new String(buffer, StandardCharsets.UTF_8);
    }

    private int receiveFilenameLength(DataInputStream input) throws IOException {
        return input.readInt();
    }
}
