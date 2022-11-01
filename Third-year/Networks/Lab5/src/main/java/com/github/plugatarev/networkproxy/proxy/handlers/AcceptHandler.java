package com.github.plugatarev.networkproxy.proxy;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

import com.github.plugatarev.networkproxy.network.Connection;
import com.github.plugatarev.networkproxy.socks.handlers.SocksConnectHandler;
import org.apache.log4j.Logger;

public final class AcceptHandler extends Handler {
    private static final Logger logger = Logger.getLogger(AcceptHandler.class);

    private final ServerSocketChannel serverSocketChannel;

    public AcceptHandler(ServerSocketChannel serverSocketChannel) {
        super(null);
        this.serverSocketChannel = serverSocketChannel;
    }

    @Override
    public void handle(SelectionKey selectionKey) throws IOException {
        SocketChannel socketChannel = serverSocketChannel.accept();
        socketChannel.configureBlocking(false);
        Connection connection = new Connection(getBufSize());
        SocksConnectHandler connectHandler = new SocksConnectHandler(connection);
        SelectionKey key = socketChannel.register(selectionKey.selector(), SelectionKey.OP_READ, connectHandler);
        connection.registerListener(() -> key.interestOpsOr(SelectionKey.OP_WRITE));
        logger.debug("New connection: " + socketChannel.getRemoteAddress());
    }
}
