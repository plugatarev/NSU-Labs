package com.github.plugatarev.networkproxy.proxy.handlers;

import com.github.plugatarev.networkproxy.network.Connection;
import com.github.plugatarev.networkproxy.socks.message.SocksResponse;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

public final class ConnectServerHandler extends Handler {
    private static final int ANY_PORT = 0;

    public ConnectServerHandler(Connection connection) {
        super(connection);
    }

    @Override
    public void handle(SelectionKey selectionKey) throws IOException {
        SocketChannel socketChannel = (SocketChannel) selectionKey.channel();
        Handler handler = (Handler) selectionKey.attachment();
        Connection connection = handler.getConnection();
        socketChannel.finishConnect();
        selectionKey.attach(new CommunicationHandler(connection));
        selectionKey.interestOpsAnd(~SelectionKey.OP_CONNECT);
        selectionKey.interestOpsOr(SelectionKey.OP_READ);
    }

    public static void connectToServer(SelectionKey clientKey, InetSocketAddress hostAddress) throws IOException {
        Handler handler = (Handler) clientKey.attachment();
        Connection clientConnection = handler.getConnection();
        SocketChannel hostSocketChannel = initServerSocket(clientConnection, clientKey, hostAddress);
        putResponseIntoBuffer(clientConnection, hostSocketChannel);
        clientKey.interestOpsOr(SelectionKey.OP_WRITE);
        clientKey.attach(new CommunicationHandler(clientConnection));
        clientConnection.getOutputBuffer().getByteBuffer().clear();
    }

    private static SocketChannel initServerSocket(Connection clientConnection, SelectionKey selectionKey, InetSocketAddress serverAddress) throws IOException {
        SocketChannel serverSocket = SocketChannel.open();
        serverSocket.bind(new InetSocketAddress(ANY_PORT));
        serverSocket.configureBlocking(false);
        Connection serverConnection = new Connection(clientConnection.getOutputBuffer(), clientConnection.getInputBuffer());
        serverSocket.connect(serverAddress);
        ConnectServerHandler connectHandler = new ConnectServerHandler(serverConnection);
        clientConnection.setChannel(serverSocket);
        serverConnection.setChannel((SocketChannel) selectionKey.channel());
        SelectionKey key = serverSocket.register(selectionKey.selector(), SelectionKey.OP_CONNECT, connectHandler);
        serverConnection.registerChanger(() -> key.interestOpsOr(SelectionKey.OP_WRITE));
        return serverSocket;
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