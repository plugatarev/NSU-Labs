package com.github.plugatarev.multicastreceiver;

import com.github.plugatarev.datatransfer.NetNode;
import org.apache.log4j.Logger;
import com.github.plugatarev.client.model.Game;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.messages.MessageParser;
import com.github.plugatarev.messages.messages.Message;
import com.github.plugatarev.messages.messages.MessageType;
import com.github.plugatarev.messages.messages.AnnouncementMessage;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketTimeoutException;
import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.Map;

public final class MulticastReceiver {
    private static final Logger logger = Logger.getLogger(MulticastReceiver.class);
    private static final int BUFFER_SIZE = 4096;
    private static final int SO_TIMEOUT_MS = 3000;

    private final View view;
    private final Game game;
    private final InetSocketAddress multicastInfo;
    private final NetworkInterface networkInterface;
    private final Thread checkerThread;

    private final Map<GameInfo, Instant> gameInfos = new HashMap<>();

    public MulticastReceiver(InetSocketAddress multicastInfo, View view, Game game, NetworkInterface networkInterface) {
        validateAddress(multicastInfo.getAddress());
        this.multicastInfo = multicastInfo;
        this.networkInterface = networkInterface;
        this.view = view;
        this.game = game;
        this.checkerThread = new Thread(getChecker());
    }

    private void validateAddress(InetAddress multicastAddress) {
        if (!multicastAddress.isMulticastAddress()) {
            throw new IllegalArgumentException(multicastAddress + " is not multicast address");
        }
    }

    public void start() {
        checkerThread.start();
    }

    public void stop() {
        checkerThread.interrupt();
    }

    private Runnable getChecker() {
        return () -> {
            try (MulticastSocket socket = new MulticastSocket(multicastInfo.getPort())) {
                byte[] buffer = new byte[BUFFER_SIZE];

                socket.joinGroup(multicastInfo, networkInterface);
                socket.setSoTimeout(SO_TIMEOUT_MS);

                while (!Thread.currentThread().isInterrupted()) {
                    DatagramPacket datagramPacket = new DatagramPacket(buffer, BUFFER_SIZE);
//                    boolean timeoutException = false;

                    try {
                        socket.receive(datagramPacket);
                        NetNode sender = new NetNode(datagramPacket.getAddress(), datagramPacket.getPort());
                        Message message = MessageParser.deserializeMessage(datagramPacket);
                        if (MessageType.ANNOUNCEMENT.equals(message.getType())) {
                            gameInfos.put(createGameInfo(sender, (AnnouncementMessage) message), Instant.now());
                        }
                    }
                    catch (SocketTimeoutException exception) {
//                        timeoutException = true;
                    }
//TODO:
//                    if (!timeoutException) {
//                        NetNode sender = new NetNode(datagramPacket.getAddress(), datagramPacket.getPort());
//                        Message message = MessageParser.deserializeMessage(datagramPacket);
//                        if (MessageType.ANNOUNCEMENT.equals(message.getType())) {
//                            gameInfos.put(createGameInfo(sender, (AnnouncementMessage) message), Instant.now());
//                        }
//                    }

                    gameInfos.entrySet().removeIf(entry ->
                            Duration.between(entry.getValue(), Instant.now()).abs().toMillis() >= SO_TIMEOUT_MS);
                    game.updateActiveGames(gameInfos.keySet());
                    view.updateGameList(gameInfos.keySet());
                }
                socket.leaveGroup(multicastInfo, networkInterface);
            } catch (IOException exception) {
                logger.error("Problem with multicast socket on port=" + multicastInfo.getPort(), exception);
            }
        };
    }

    private GameInfo createGameInfo(NetNode sender, AnnouncementMessage announcementMsg) {
        return new GameInfo(announcementMsg.getConfig(), sender, announcementMsg.getPlayers(), announcementMsg.isCanJoin());
    }
}