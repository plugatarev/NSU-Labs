package com.github.plugatarev.networkproxy.proxy.handlers;

import com.github.plugatarev.networkproxy.network.Connection;
import com.github.plugatarev.networkproxy.socks.message.SocksResponse;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

public final class ConnectHandler extends Handler {
    private static final int ANY_PORT = 0;

    public ConnectHandler(Connection connection) {
        super(connection);
    }

    @Override
    public void handle(SelectionKey selectionKey) throws IOException {
        SocketChannel socketChannel = (SocketChannel) selectionKey.channel();
        Handler handler = (Handler) selectionKey.attachment();
        Connection connection = handler.getConnection();
        socketChannel.finishConnect();
        selectionKey.attach(new ForwardHandler(connection));
        selectionKey.interestOpsAnd(~SelectionKey.OP_CONNECT);
        selectionKey.interestOpsOr(SelectionKey.OP_READ);
    }

    public static void connectHost(SelectionKey clientKey, InetSocketAddress hostAddress) throws IOException {
        Handler handler = (Handler) clientKey.attachment();
        Connection clientConnection = handler.getConnection();
        SocketChannel hostSocketChannel = initHostSocket(clientConnection, clientKey, hostAddress);
        putResponseIntoBuffer(clientConnection, hostSocketChannel);
        clientKey.interestOpsOr(SelectionKey.OP_WRITE);
        clientKey.attach(new ForwardHandler(clientConnection));
        clientConnection.getOutputBuffer().getByteBuffer().clear();
    }

    private static SocketChannel initHostSocket(Connection clientConnection, SelectionKey selectionKey, InetSocketAddress hostAddress) throws IOException {
        SocketChannel hostSocket = SocketChannel.open();
        hostSocket.bind(new InetSocketAddress(ANY_PORT));
        hostSocket.configureBlocking(false);
        Connection hostConnection = new Connection(clientConnection.getOutputBuffer(), clientConnection.getInputBuffer());
        hostSocket.connect(hostAddress);
        ConnectHandler connectHandler = new ConnectHandler(hostConnection);
        clientConnection.setChannel(hostSocket);
        hostConnection.setChannel((SocketChannel) selectionKey.channel());
        SelectionKey key = hostSocket.register(selectionKey.selector(), SelectionKey.OP_CONNECT, connectHandler);
        hostConnection.registerListener(() -> key.interestOpsOr(SelectionKey.OP_WRITE));
        return hostSocket;
    }

    private static void putResponseIntoBuffer(Connection connection, SocketChannel socketChannel) throws IOException {
        InetSocketAddress socketAddress = (InetSocketAddress) socketChannel.getLocalAddress();
        SocksResponse response = new SocksResponse();
        byte[] address = InetAddress.getLocalHost().getAddress();
        response.setBoundIp4Address(address);
        response.setBoundPort((short) socketAddress.getPort());
        ByteBuffer inputBuff = connection.getInputBuffer().getByteBuffer();
        inputBuff.put(response.toByteBuffer());
    }
}
