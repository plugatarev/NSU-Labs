package com.github.plugatarev.networkproxy.socksHandlers;

import java.io.IOException;
import java.nio.channels.SelectionKey;

import com.github.plugatarev.networkproxy.handlers.Handler;
import com.github.plugatarev.networkproxy.network.Connection;

public abstract class SocksHandler extends Handler {
    public SocksHandler(Connection connection) {
        super(connection);
    }

    @Override
    public int read(SelectionKey selectionKey) throws IOException {
        int readCount = super.read(selectionKey);
        if (readCount < 0) {
            throw new IOException("Socket closed during SOCKS5 handshake");
        }
        return readCount;
    }
}
