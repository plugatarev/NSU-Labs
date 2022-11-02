package com.github.plugatarev.networkproxy.network;

import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

@RequiredArgsConstructor
public final class Connection {
    private static final Logger logger = Logger.getLogger(Connection.class);

    @Getter private final ByteBufferWrapper inputBuffer;
    @Getter private final ByteBufferWrapper outputBuffer;

    @Setter private SocketChannel channel;
    private int writeStartPosition = 0;

    public Connection(int bufSize) {
        this.inputBuffer = new ByteBufferWrapper(ByteBuffer.allocate(bufSize));
        this.outputBuffer = new ByteBufferWrapper(ByteBuffer.allocate(bufSize));
    }

    public void registerListener(ByteBufferWrapper.Listener listener) {
        inputBuffer.setListener(listener);
    }

    public void notifyBufferListener() {
        outputBuffer.notifyListener();
    }

    public void closeChannel() throws IOException {
        if (channel != null) {
            logger.debug("Socket closed: " + channel.getRemoteAddress());
            channel.close();
        }
    }

    public void shutdown() {
        outputBuffer.setShutdown(true);
    }

    public boolean isChannelShutDown() {
        return inputBuffer.isReadyClose();
    }

    public void prepareToWrite() {
        getInputBuffer().getByteBuffer().flip().position(writeStartPosition);
    }

    public boolean isReadyToClose() {
        return inputBuffer.isReadyClose() && outputBuffer.isReadyClose();
    }

    public void resetWriteStartPosition() {
        writeStartPosition = 0;
    }

    public void shiftWriteStartPosition() {
        ByteBuffer inputBuffer = getInputBuffer().getByteBuffer();
        this.writeStartPosition = inputBuffer.position();
        int newStartPosition = inputBuffer.limit();
        inputBuffer.clear();
        inputBuffer.position(newStartPosition);
    }

    @RequiredArgsConstructor
    public static final class ByteBufferWrapper {
        @Getter private final ByteBuffer byteBuffer;

        @Setter private Listener listener;
        @Setter private boolean shutdown = false;

        public void notifyListener(){
            listener.update();
        }

        public boolean isReadyClose(){
            return shutdown && (byteBuffer.remaining() == 0);
        }

        public interface Listener {
            void update();
        }
    }
}
