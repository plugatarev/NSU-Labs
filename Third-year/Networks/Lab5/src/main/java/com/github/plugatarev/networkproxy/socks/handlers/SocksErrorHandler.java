package com.github.plugatarev.networkproxy.socksHandlers;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

import com.github.plugatarev.networkproxy.handlers.Handler;
import com.github.plugatarev.networkproxy.network.Connection;

public final class SocksErrorHandler extends Handler {
    public SocksErrorHandler(Connection connection) {
        super(connection);
    }

    @Override
    public void handle(SelectionKey selectionKey) {
    }

    @Override
    public int write(SelectionKey selectionKey) throws IOException {
        int remaining = super.write(selectionKey);
        if (remaining == 0) {
            SocketChannel socket = (SocketChannel) selectionKey.channel();
            socket.close();
        }
        return remaining;
    }
}
