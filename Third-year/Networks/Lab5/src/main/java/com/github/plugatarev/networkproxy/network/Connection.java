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

    @Getter private final ObservableByteBuffer inputBuffer;
    @Getter private final ObservableByteBuffer outputBuffer;

    @Setter private SocketChannel channel;
    private int writeStartPosition = 0;

    public Connection(int bufSize) {
        this.inputBuffer = new ObservableByteBuffer(ByteBuffer.allocate(bufSize));
        this.outputBuffer = new ObservableByteBuffer(ByteBuffer.allocate(bufSize));
    }

    public void registerBufferListener(ObservableByteBuffer.BufferListener bufferListener) {
        this.inputBuffer.setBufferListener(bufferListener);
    }

    public void notifyBufferListener() {
        this.outputBuffer.notifyListener();
    }

    public void closeChannel() throws IOException {
        if (null != this.channel) {
            logger.debug("Socket closed: " + this.channel.getRemoteAddress());
            this.channel.close();
        }
    }

    public void shutdown() {
        this.outputBuffer.shutdown();
    }

    public boolean isChannelShutDown() {
        return this.inputBuffer.isReadyToClose();
    }

    public void prepareToWrite() {
        ByteBuffer inputBuffer = getInputBuffer().getByteBuffer();
        inputBuffer.flip();
        inputBuffer.position(this.writeStartPosition);
    }

    public boolean isReadyToClose() {
        return this.inputBuffer.isReadyToClose() && this.outputBuffer.isReadyToClose();
    }

    public void resetWriteStartPosition() {
        this.writeStartPosition = 0;
    }

    public void setWriteStartPosition() {
        ByteBuffer inputBuffer = getInputBuffer().getByteBuffer();
        this.writeStartPosition = inputBuffer.position();
        int newStartPosition = inputBuffer.limit();
        inputBuffer.clear();
        inputBuffer.position(newStartPosition);
    }

    @RequiredArgsConstructor
    public static final class ObservableByteBuffer {
        @Getter private final ByteBuffer byteBuffer;

        @Setter private BufferListener bufferListener;
        private boolean shouldShutdown = false;

        public void notifyListener(){
            this.bufferListener.onUpdate();
        }

        public void shutdown() {
            this.shouldShutdown = true;
        }

        public boolean isReadyToClose(){
            return (this.byteBuffer.remaining() == 0) && this.shouldShutdown;
        }

        public interface BufferListener {
            void onUpdate();
        }
    }
}
