package com.github.plugatarev.networkproxy.network;

import com.github.plugatarev.networkproxy.handlers.ConnectHandler;
import com.github.plugatarev.networkproxy.handlers.Handler;
import com.github.plugatarev.networkproxy.handlers.SocksRequestHandler;
import com.github.plugatarev.networkproxy.socks.SocksRequest;
import org.apache.log4j.Logger;
import org.xbill.DNS.Record;
import org.xbill.DNS.*;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class DnsService {
    private static final Logger logger = Logger.getLogger(DnsService.class);

    private static final byte HOST_UNREACHABLE_ERROR = 0x04;
    private static final int DNS_SERVER_PORT = 53;
    private static final int BUFFER_SIZE = 1024;
    private static final int CACHE_SIZE = 256;

    private static DnsService INSTANCE;

    private final Map<Integer, DnsMapValue> unresolvedNames = new HashMap<>();
    private final Cache resolvedNamesCache = new Cache(CACHE_SIZE);
    private final InetSocketAddress dnsServerAddress;

    private Handler dnsResponseHandler;
    private DatagramChannel datagramChannel;
    private int messageID = 0;

    public static DnsService create() {
        if (INSTANCE == null) {
            INSTANCE = new DnsService();
        }
        return INSTANCE;
    }

    private DnsService() {
        this.dnsServerAddress = new InetSocketAddress(ResolverConfig.getCurrentConfig().server().getAddress(), DNS_SERVER_PORT);
    }

    public void setDatagramChannel(DatagramChannel channel) {
        this.datagramChannel = channel;
        initResponseHandler();
    }

    public void registerSelector(Selector selector) throws ClosedChannelException {
        this.datagramChannel.register(selector, SelectionKey.OP_READ, dnsResponseHandler);
    }

    public void resolveName(SocksRequest request, SelectionKey selectionKey) throws IOException {
        try {
            String name = request.getDomainName();
            String cachedAddress = resolvedNamesCache.get(name + ".");

            if (null != cachedAddress) {
                connectToTarget(cachedAddress, selectionKey, request.getDestinationPort());
                return;
            }

            logger.debug("New domain name to resolve: " + request.getDomainName());
            DnsMapValue mapValue = new DnsMapValue(selectionKey, request.getDestinationPort());
            Message query = getQuery(name);
            byte[] queryBytes = query.toWire();
            unresolvedNames.put(query.getHeader().getID(), mapValue);
            datagramChannel.send(ByteBuffer.wrap(queryBytes), dnsServerAddress);
        }
        catch (TextParseException exception) {
            SocksRequestHandler.onError(selectionKey, HOST_UNREACHABLE_ERROR);
            logger.error(exception);
        }
    }

    private void initResponseHandler() {
        this.dnsResponseHandler = new Handler(null) {
            @Override
            public void handle(SelectionKey selectionKey) throws IOException {
                ByteBuffer byteBuffer = ByteBuffer.allocate(BUFFER_SIZE);

                if (datagramChannel.receive(byteBuffer) == null) {
                    return;
                }

                Message response = new Message(byteBuffer.flip().array());
                List<Record> answers = response.getSection(Section.ANSWER);
                int responseID = response.getHeader().getID();
                DnsMapValue unresolvedName = unresolvedNames.get(response.getHeader().getID());

                if (answers.size() == 0) {
                    SocksRequestHandler.onError(unresolvedName.selectionKey(), HOST_UNREACHABLE_ERROR);
                    return;
                }

                String hostname = response.getQuestion().getName().toString();
                logger.debug(hostname + " resolved");
                String address = answers.get(0).rdataToString();
                resolvedNamesCache.put(hostname, address);
                connectToTarget(address, unresolvedName.selectionKey(), unresolvedName.targetPort());
                unresolvedNames.remove(responseID);
            }
        };
    }

    private void connectToTarget(String address, SelectionKey selectionKey, int port) throws IOException {
        InetSocketAddress socketAddress = new InetSocketAddress(address, port);
        ConnectHandler.connectToTarget(selectionKey, socketAddress);
    }

    private Message getQuery(String domainName) throws TextParseException {
        Header header = new Header(messageID++);
        header.setFlag(Flags.RD);
        header.setOpcode(0);

        Message message = new Message();
        message.setHeader(header);

        Record record = Record.newRecord(new Name(domainName + "."), Type.A, DClass.IN);
        message.addRecord(record, Section.QUESTION);

        return message;
    }

    private record DnsMapValue(SelectionKey selectionKey,short targetPort) {
    }

    private static final class Cache {
        private final int capacity;
        private final HashMap<String, String> resolvedNames = new HashMap<>(CACHE_SIZE);

        public Cache(int capacity) {
            this.capacity = capacity;
        }

        public String get(String key){
            return resolvedNames.get(key);
        }

        public void put(String key, String value){
            if (resolvedNames.size() >= capacity) {
                resolvedNames.remove(resolvedNames.entrySet().iterator().next().getKey());
            }
            this.resolvedNames.put(key, value);
        }
    }
}