package com.github.plugatarev.networkproxy.proxy;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

import com.github.plugatarev.networkproxy.network.Connection;
import lombok.Getter;
import lombok.RequiredArgsConstructor;
import org.apache.log4j.Logger;

@RequiredArgsConstructor
public abstract class Handler {
    private final static int EMPTY = 0;
    private static final Logger logger = Logger.getLogger(Handler.class);
    private static final int BUF_SIZE = 65536;

    @Getter private final Connection connection;

    public abstract void handle(SelectionKey selectionKey) throws IOException;

    public static int getBufSize() {
        return BUF_SIZE;
    }

    public int read(SelectionKey selectionKey) throws IOException {
        Handler handler = (Handler) selectionKey.attachment();
        SocketChannel socket = (SocketChannel) selectionKey.channel();
        ByteBuffer outputBuffer = handler.getConnection().getOutputBuffer().getByteBuffer();

        if (!isReadyToRead(outputBuffer, connection)) {
            return 0;
        }

        int readCount = socket.read(outputBuffer);

        if (readCount <= 0) {
            connection.shutdown();
            selectionKey.interestOps(EMPTY);
            checkConnectionClose(socket);
        }

        return readCount;
    }

    public int write(SelectionKey selectionKey) throws IOException {
        ByteBuffer inputBuffer = connection.getInputBuffer().getByteBuffer();
        SocketChannel socketChannel = (SocketChannel) selectionKey.channel();
        connection.prepareToWrite();
        socketChannel.write(inputBuffer);
        int remaining = inputBuffer.remaining();

        if (remaining == EMPTY) {
            selectionKey.interestOps(SelectionKey.OP_READ);
            checkChannel(socketChannel, inputBuffer);
        }
        else {
            connection.shiftWriteStartPosition();
        }
        return remaining;
    }

    private boolean isReadyToRead(ByteBuffer buffer, Connection connection) {
        return (buffer.position() < BUF_SIZE / 2) || connection.isChannelShutDown();
    }

    private void checkConnectionClose(SocketChannel socketChannel) throws IOException {
        if (connection.isReadyToClose()) {
            logger.debug("Socket closed: " + socketChannel.getRemoteAddress());
            socketChannel.close();
            connection.closeChannel();
        }
    }

    private void checkChannel(SocketChannel socketChannel, ByteBuffer buffer) throws IOException {
        if (connection.isChannelShutDown()) {
            socketChannel.shutdownOutput();
            return;
        }

        buffer.clear();
        connection.resetWriteStartPosition();
    }
}
