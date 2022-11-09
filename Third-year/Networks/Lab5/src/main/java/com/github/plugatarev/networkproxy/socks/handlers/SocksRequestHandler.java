package com.github.plugatarev.networkproxy.socks.handlers;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;

import com.github.plugatarev.networkproxy.proxy.handlers.ConnectServerHandler;
import com.github.plugatarev.networkproxy.proxy.handlers.Handler;
import com.github.plugatarev.networkproxy.network.Connection;
import com.github.plugatarev.networkproxy.network.DNS;
import com.github.plugatarev.networkproxy.socks.SocksParser;
import com.github.plugatarev.networkproxy.socks.message.SocksRequest;
import com.github.plugatarev.networkproxy.socks.message.SocksResponse;

public final class SocksRequestHandler extends Handler {
    private static final byte DOMAIN_NAME_TYPE = 0x03;
    private static final int RSV = 0x00;

    public SocksRequestHandler(Connection connection) {
        super(connection);
    }

    @Override
    public void handle(SelectionKey selectionKey) throws IOException {
        ByteBuffer outputBuffer = getConnection().getOutputBuffer().getByteBuffer();

        read(selectionKey);
        SocksRequest request = SocksParser.parseRequest(outputBuffer);
        if (request == null) return;

        byte rsv = request.getRsv();

        if (rsv != RSV) {
            error(selectionKey, rsv);
            return;
        }

        if (request.getAddressType() == DOMAIN_NAME_TYPE) {
            DNS dns = DNS.create();
            dns.resolveName(request, selectionKey);
            return;
        }
        ConnectServerHandler.connectToServer(selectionKey, request.getAddress());
    }

    public static void error(SelectionKey selectionKey, byte error) {
        Handler handler = (Handler) selectionKey.attachment();
        Connection connection = handler.getConnection();
        putErrorResponseIntoBuf(selectionKey, connection, error);
        selectionKey.attach(new SocksErrorHandler(connection));
    }

    private static void putErrorResponseIntoBuf(SelectionKey selectionKey, Connection connection, byte error) {
        SocksResponse response = new SocksResponse();
        response.setReplyCode(error);
        ByteBuffer inputBuff = connection.getInputBuffer().getByteBuffer();
        inputBuff.put(response.toByteBufferWithoutAddress());
        connection.getOutputBuffer().getByteBuffer().clear();
        selectionKey.interestOpsOr(SelectionKey.OP_WRITE);
    }
}