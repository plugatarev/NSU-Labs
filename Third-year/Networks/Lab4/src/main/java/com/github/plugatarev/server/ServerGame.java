package com.github.plugatarev.server;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.NodeRole;
import com.github.plugatarev.datatransfer.GameSocket;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.datatransfer.RDTSocket;
import com.github.plugatarev.gamehandler.GameHandler;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.game.Game;
import com.github.plugatarev.messages.MessageHandler;
import lombok.Getter;
import org.apache.log4j.Logger;
import com.github.plugatarev.messages.messages.AckMessage;
import com.github.plugatarev.messages.messages.AnnouncementMessage;
import com.github.plugatarev.messages.messages.ErrorMessage;
import com.github.plugatarev.messages.messages.JoinMessage;
import com.github.plugatarev.messages.messages.Message;
import com.github.plugatarev.messages.messages.PingMessage;
import com.github.plugatarev.messages.messages.RoleChangeMessage;
import com.github.plugatarev.messages.messages.StateMessage;
import com.github.plugatarev.messages.messages.SteerMessage;
import com.github.plugatarev.utils.PlayerUtils;

import javax.validation.constraints.NotNull;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;
import java.time.Duration;
import java.time.Instant;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicLong;

public final class ServerGame implements ServerHandler {
    private static final int EMPTY = -1;
    private static final Logger logger = Logger.getLogger(ServerGame.class);
    public static final int ANNOUNCEMENT_SEND_PERIOD_MS = 1000;

    private final GameHandler game;
    private final SnakesProto.GameConfig gameConfig;
    private final Map<Player, SnakesProto.Direction> playersMoves = new ConcurrentHashMap<>();
    private final Map<Player, Instant> playersLastSeen = new ConcurrentHashMap<>();
    private final AtomicLong msgSeq = new AtomicLong(0);
    private final MessageHandler messageHandler = createMessageHandler();
    private final ExecutorService executorService = Executors.newFixedThreadPool(1);
    //TODO:
    private final String gameName = "test";

    @Getter private final RDTSocket socket;
    private final InetSocketAddress multicastAddress;

    private Future<?> activeDeputyTask = null;
    private Timer timer = new Timer();
    private Thread receiveThread = null;
    private Player masterPlayer = null;
    private Player deputyPlayer = null;
    private Player potentialDeputyPlayer = null;
    private boolean isDeputyBeingChosen = false;

    public ServerGame(SnakesProto.GameConfig gameConfig, InetSocketAddress multicastAddress, InetAddress masterAddress, int masterPort, String masterName, NetworkInterface networkInterface) throws IOException {
        this.gameConfig = gameConfig;
        this.game = new Game(gameConfig, this);
        this.multicastAddress = multicastAddress;
        this.socket = new GameSocket(networkInterface, this.gameConfig.getStateDelayMs() / 10);
        this.socket.start();

        Optional<Player> playerOptional = registerNewPlayer(new NetNode(masterAddress, masterPort), masterName);
        playerOptional.ifPresent(player -> {
            player.setRole(SnakesProto.NodeRole.MASTER);
            player.setScore(0);
            masterPlayer = player;
        });

        startTimerTasks();
        startReceivingMessages();
    }

    public ServerGame(GameState gameState, InetSocketAddress multicastAddress, NetworkInterface networkInterface) throws IOException {
        gameConfig = gameState.getGameConfig();
        game = new Game(gameState, this);
        this.multicastAddress = multicastAddress;
        socket = new GameSocket(networkInterface, gameConfig.getStateDelayMs() / 10);
        socket.start();

        gameState.getActivePlayers().forEach(player -> {
            player.setRole(NodeRole.MASTER.equals(player.getRole()) ? NodeRole.NORMAL : player.getRole());
            player.setRole(NodeRole.DEPUTY.equals(player.getRole()) ? NodeRole.MASTER : player.getRole());
            if (NodeRole.MASTER.equals(player.getRole())) {
                masterPlayer = player;
            }
            this.playersLastSeen.put(player, Instant.now());
            if (!NodeRole.VIEWER.equals(player.getRole())) {
                playersMoves.put(player, game.getSnakeByPlayer(player).getDirection());
            }
        });

        startTimerTasks();
        startReceivingMessages();
    }

    @Override
    public void update(GameState gameState) {
        var playerList = new ArrayList<>(playersLastSeen.keySet());
        this.playersLastSeen.keySet()
                .forEach(player -> socket.sendWithoutConfirm(
//                            new StateMessage(gameState, playerList, msgSeq.getAndIncrement(), masterPlayer.getId(), player.getId()),
                                new StateMessage(gameState, msgSeq.getAndIncrement(), masterPlayer.getId(), player.getId()),
                            player.getNetNode()
                        )
                );
        if (gameState.getActivePlayers().isEmpty()) {
            stop();
        }
    }

    @Override
    public int getPort() {
        return socket.getLocalPort();
    }

    @Override
    public void stop() {
        if (deputyPlayer != null) {
            socket.sendWithoutConfirm(
                    new RoleChangeMessage(NodeRole.MASTER, NodeRole.MASTER, msgSeq.getAndIncrement(), masterPlayer.getId(), deputyPlayer.getId()),
                    deputyPlayer.getNetNode()
            );
        }
        timer.cancel();
        receiveThread.interrupt();
        socket.stop();
    }

    private void startTimerTasks() {
        timer.cancel();
        timer = new Timer();
        startGameUpdateTimer();
        startSendAnnouncementMessages();
        startRemovingDisconnectedPlayers();
    }

    private void startGameUpdateTimer() {
        this.timer.schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        game.moveAllSnakes(Map.copyOf(playersMoves));
                        playersMoves.clear();
                    }
                },
                0, gameConfig.getStateDelayMs());
    }

    private void startSendAnnouncementMessages() {
        this.timer.schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        socket.sendWithoutConfirm(
                                new AnnouncementMessage(gameConfig, new ArrayList<>(playersLastSeen.keySet()), true, gameName, msgSeq.getAndIncrement()),
                                new NetNode(multicastAddress.getAddress(), multicastAddress.getPort())
                        );
                    }
                },
                0, ANNOUNCEMENT_SEND_PERIOD_MS);
    }

    private void startRemovingDisconnectedPlayers() {
        timer.schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        playersLastSeen.forEach((player, lastSeen) -> {
                            if (isDisconnected(lastSeen)) {
                                game.removePlayer(player);
                                playersMoves.remove(player);
                            }
                        });
                        playersLastSeen.entrySet().removeIf(entry -> isDisconnected(entry.getValue()));

                        if (potentialDeputyPlayer != null && !playersLastSeen.containsKey(potentialDeputyPlayer)) {
                            potentialDeputyPlayer = null;
                            isDeputyBeingChosen = false;
                            chooseNewDeputy();
                        }
                        if (deputyPlayer != null && !playersLastSeen.containsKey(deputyPlayer)) {
                            deputyPlayer = null;
                            chooseNewDeputy();
                        }
                    }
                },
                0,
                gameConfig.getStateDelayMs() / 10);
    }

    private void startReceivingMessages() {
        this.receiveThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                var recv = socket.receive();
                handleMessage(recv.getOwner(), recv.getMessage());
            }
        });
        receiveThread.start();
    }

    private void registerNewMove(Player player, SnakesProto.Direction direction) {
        playersMoves.put(player, direction);
    }

    private void handleMessage(NetNode sender, Message message) {
        switch (message.getType()) {
            case ROLE_CHANGE -> messageHandler.handle(sender, (RoleChangeMessage) message);
            case STEER -> messageHandler.handle(sender, (SteerMessage) message);
            case JOIN -> messageHandler.handle(sender, (JoinMessage) message);
            case PING -> messageHandler.handle(sender, (PingMessage) message);
            case ERROR -> messageHandler.handle(sender, (ErrorMessage) message);
            default -> throw new IllegalStateException("Server: Cant handle this message type = " + message.getType());
        }
    }

    private void chooseNewDeputy() {
        if (activeDeputyTask != null) {
            activeDeputyTask.cancel(true);
        }
        isDeputyBeingChosen = true;
        activeDeputyTask = executorService.submit(() -> {
            Optional<Player> playerOptional = playersLastSeen.keySet().stream().filter(player -> player.getRole() != NodeRole.MASTER).findAny();
            playerOptional.ifPresentOrElse(
                    this::setDeputyPlayer,
                    () -> {
                        logger.warn("Cant chose deputy");
                        isDeputyBeingChosen = false;
                    }
            );
        });
    }

    private void setDeputyPlayer(@NotNull Player deputy) {
        potentialDeputyPlayer = deputy;
        Message response = socket.send(
                new RoleChangeMessage(NodeRole.MASTER, NodeRole.DEPUTY, msgSeq.getAndIncrement(), masterPlayer.getId(), deputy.getId()),
                deputy.getNetNode()
        );
        if (response instanceof AckMessage) {
            deputyPlayer = deputy;
            deputyPlayer.setRole(NodeRole.DEPUTY);
        }
        potentialDeputyPlayer = null;
        isDeputyBeingChosen = false;
    }

    private boolean isDisconnected(@NotNull Instant moment) {
        return Duration.between(moment, Instant.now()).abs().toMillis() >= gameConfig.getStateDelayMs() * 0.8;
    }

    private boolean validateNewPlayer(NetNode sender, JoinMessage joinMsg) {
        if (playersLastSeen.keySet().stream().anyMatch(player -> player.getName().equals(joinMsg.getPlayerName()))) {
            logger.error("Node=" + sender + ": " + "Player with name '" + joinMsg.getPlayerName() + "' already registered as player");
            this.socket.sendWithoutConfirm(
                    new ErrorMessage("Player with name already exist", joinMsg.getMessageSequence(), masterPlayer.getId(), -1),
                    sender
            );
            return false;
        }
        return true;
    }

    private Optional<Player> registerNewPlayer(NetNode netNode, String playerName) {
        try {
            Player player = game.registerNewPlayer(playerName, netNode);
            playersLastSeen.put(player, Instant.now());
            playersMoves.put(player, game.getSnakeByPlayer(player).getDirection());
            return Optional.of(player);
        }
        catch (IllegalStateException exception) {
            logger.debug("Cant place player on field because no space");
            String errorMessage = "Cant place player on field because no space";
            this.socket.sendWithoutConfirm(
                    new ErrorMessage(errorMessage, msgSeq.getAndIncrement(), masterPlayer.getId(), EMPTY),
                    netNode
            );
            return Optional.empty();
        }
    }

    private void removePlayer(NetNode sender) {
        checkRegistration(sender);
        Player player = PlayerUtils.findPlayerByAddress(sender, playersLastSeen.keySet());
        if (player == null) {
            return;
        }

        updateLastSeen(sender);
        playersMoves.remove(player);
        game.removePlayer(player);
    }

    private void checkRegistration(@NotNull NetNode sender) {
        if (null == PlayerUtils.findPlayerByAddress(sender, playersLastSeen.keySet())) {
            logger.error("Node=" + sender + " is not registered");
            throw new IllegalArgumentException("Node={" + sender + "} is not registered");
        }
    }

    private void updateLastSeen(@NotNull NetNode sender) {
        Player player = PlayerUtils.findPlayerByAddress(sender, playersLastSeen.keySet());
        if (player != null) {
            playersLastSeen.put(player, Instant.now());
        }
    }

    private MessageHandler createMessageHandler() {
        return new MessageHandler() {
            @Override
            public void handle(NetNode sender, SteerMessage message) {
                checkRegistration(sender);
                updateLastSeen(sender);
                Player player = PlayerUtils.findPlayerByAddress(sender, playersLastSeen.keySet());
                if (player == null) {
                    return;
                }

                registerNewMove(player, message.getDirection());
                logger.debug("NetNode=" + sender + " as player=" + player + " make move with direction=" + message.getDirection());
            }

            @Override
            public void handle(NetNode sender, JoinMessage message) {
                if (!validateNewPlayer(sender, message)) {
                    return;
                }
                registerNewPlayer(sender, message.getPlayerName())
                        .ifPresent(player -> {
                                    logger.debug("NetNode=" + sender + " was successfully registered as player=" + player);
                                    player.setRole(NodeRole.NORMAL);
                                    player.setScore(0);
                                    if (deputyPlayer == null && !isDeputyBeingChosen) {
                                        chooseNewDeputy();
                                    }
                                }
                        );
            }

            @Override
            public void handle(NetNode sender, PingMessage message) {
                updateLastSeen(sender);
            }

            @Override
            public void handle(NetNode sender, StateMessage message) {
                throw new IllegalStateException("Server shouldn't receive State messages");
            }

            @Override
            public void handle(NetNode sender, ErrorMessage message) {
                logger.error(message.getErrorMessage());
            }

            @Override
            public void handle(NetNode sender, RoleChangeMessage message) {
                if (NodeRole.VIEWER.equals(message.getSenderRole()) && NodeRole.MASTER.equals(message.getReceiverRole())) {
                    removePlayer(sender);
                }
                else {
                    logger.warn("Server: Unsupported roles at role change message=" + message + " from=" + sender);
                    throw new IllegalArgumentException("Unsupported roles at role change message=" + message + " from=" + sender);
                }
            }
        };
    }
}