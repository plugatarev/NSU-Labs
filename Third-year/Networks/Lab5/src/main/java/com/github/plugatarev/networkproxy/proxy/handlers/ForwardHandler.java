package com.github.plugatarev.networkproxy.proxy;

import java.io.IOException;
import java.nio.channels.SelectionKey;

import com.github.plugatarev.networkproxy.network.Connection;

public final class ForwardHandler extends Handler {
    public ForwardHandler(Connection connection) {
        super(connection);
    }

    @Override
    public void handle(SelectionKey selectionKey) throws IOException {
        Connection connection = ((Handler) selectionKey.attachment()).getConnection();
        int readCount = read(selectionKey);
        if (readCount != 0) {
            connection.notifyBufferListener();
        }
    }
}
