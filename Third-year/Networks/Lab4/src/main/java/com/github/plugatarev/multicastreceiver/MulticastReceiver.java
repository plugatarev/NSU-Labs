package com.github.plugatarev.multicastreceiver;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.Player;
import org.apache.log4j.Logger;
import com.github.plugatarev.client.model.Game;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.messages.MessageParser;
import com.github.plugatarev.messages.messages.Message;
import com.github.plugatarev.messages.messages.MessageType;
import com.github.plugatarev.messages.messages.AnnouncementMessage;

import java.io.IOException;
import java.net.*;
import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

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
                    try {
                        socket.receive(datagramPacket);
                        NetNode sender = new NetNode(datagramPacket.getAddress(), datagramPacket.getPort());
                        Message message = MessageParser.deserializeMessage(datagramPacket);
                        if (message.getType().equals(MessageType.ANNOUNCEMENT)) {
                            ((AnnouncementMessage) message).getGames().forEach(s -> gameInfos.put(createGameInfo(sender, s), Instant.now()));
                        }
                    }
                    catch (SocketTimeoutException ignored) {
                    }
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

    private GameInfo createGameInfo(NetNode sender, SnakesProto.GameAnnouncement game){
        List<Player> players = game.getPlayers().getPlayersList()
                                   .stream()
                                   .map(s -> {
                                       try {
                                           return new Player(s.getName(), s.getId(), new NetNode(s.getIpAddress(), s.getPort()), s.getRole(), s.getScore());
                                       } catch (UnknownHostException e) {
                                           throw new IllegalStateException("UnknownHost " + e);
                                       }
                                   })
                                   .toList();
        return new GameInfo(game.getGameName(), game.getConfig(), sender, players, game.getCanJoin());
    }
}