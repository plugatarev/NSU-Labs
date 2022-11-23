package com.github.plugatarev.client.model;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.NodeRole;
import com.github.plugatarev.SnakesProto.Direction;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.GameSocket;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.datatransfer.RDTSocket;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.messages.MessageHandler;
import com.github.plugatarev.messages.MessageOwner;
import com.github.plugatarev.messages.messages.*;
import com.github.plugatarev.multicastreceiver.GameInfo;
import com.github.plugatarev.server.ServerGame;
import com.github.plugatarev.server.ServerHandler;
import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;
import org.apache.log4j.Logger;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.utils.StateUtils;

import javax.validation.constraints.NotNull;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;
import java.time.Duration;
import java.time.Instant;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicLong;

@RequiredArgsConstructor
public final class Game implements GameHandler {
    private static final Logger logger = Logger.getLogger(Game.class);
    private static final int EMPTY = -1;

    private final RDTSocket rdtSocket;
    private final GameConfig config;
    private final String playerName;
    @Getter private final String gameName;
    private final View view;
    private final InetSocketAddress multicastInfo;
    private final NetworkInterface networkInterface;

    @Setter private NetNode serverNetNode;

    private ServerHandler activeServerGame = null;
    private GameState gameState;
    private NodeRole nodeRole;
    private NetNode master;
    private NetNode deputy;
    private Instant masterLastSeen = Instant.now();

    private int masterID = EMPTY;
    private int deputyID = EMPTY;
    private int playerID = EMPTY;

    private final AtomicLong msgSeq = new AtomicLong(0);
//    private final ExecutorService executorService = Executors.newFixedThreadPool(2);
    private final MessageHandler messageHandler = createMessageHandler();
//    private Future<?> activeJoinTask = null;
    private Timer timer = new Timer();

    private long currentSteerMessageSeq = EMPTY;
    private Direction curDirection = null;
    private Thread receiveThread = null;
    private boolean anyGamesOnLocalhost = false;

    public Game(GameConfig config, String playerName, View view, InetSocketAddress multicastInfo, NetworkInterface networkInterface) throws IOException {
        int pingDelay = config.getStateDelayMs() / 10;
        this.rdtSocket = new GameSocket(networkInterface, pingDelay);
        this.config = config;
        this.view = view;
        this.multicastInfo = multicastInfo;
        this.networkInterface = networkInterface;
        this.playerName = playerName;
        this.gameName = playerName + "Game";
    }

    @Override
    public void startNewGame() {
        if (anyGamesOnLocalhost) {
            logger.info("You have a server running on localhost. Shut it down and try again.");
            return;
        }

        stopCurrentServerGame();
        rdtSocket.stop();
        rdtSocket.start();
        try {
            activeServerGame = new ServerGame(config, multicastInfo, rdtSocket.getAddress(), rdtSocket.getLocalPort(), playerName, networkInterface, gameName);
            master = new NetNode(rdtSocket.getAddress(), activeServerGame.getPort());
            masterLastSeen = Instant.now();
            gameState = null;
            changeNodeRole(NodeRole.MASTER);

            startTimerTasks();
        }
        catch (IOException exception) {
            logger.error(exception.getLocalizedMessage());
            stopCurrentServerGame();
        }
    }

    @Override
    public void joinGame(NetNode gameOwner, String playerName) {
        exit();
        rdtSocket.start();
//        if (activeJoinTask != null) {
//            activeJoinTask.cancel(true);
//        }
//        activeJoinTask = executorService.submit(() -> {
            //TODO:
            Message response = rdtSocket.send(new JoinMessage(SnakesProto.PlayerType.HUMAN, playerName, gameName, NodeRole.NORMAL, msgSeq.get()), gameOwner);
            if (response == null) return;

            if (response.getType().equals(MessageType.ERROR)) {
                messageHandler.handle(null, (ErrorMessage) response);
                return;
            }
            if (!response.getType().equals(MessageType.ACK)) {
                logger.error("For join message, Server didn't respond with Ack message");
                return;
            }

            masterID = response.getSenderID();
            playerID = response.getReceiverID();
            master = gameOwner;
            masterLastSeen = Instant.now();
            deputy = null;
            gameState = null;
            changeNodeRole(NodeRole.NORMAL);

            startTimerTasks();
//        });
    }

    @Override
    public void joinServerPlayer(NetNode serverNetNode) {
        AnnouncementMessage announcementMessage = new AnnouncementMessage(msgSeq.getAndIncrement());
        announcementMessage.addGame(gameState.getActivePlayers(), config, gameName, true);

        rdtSocket.sendWithoutConfirm(announcementMessage, serverNetNode);
    }

    @Override
    public void handleMove(Direction direction) {
        if (!NodeRole.VIEWER.equals(nodeRole)) {
            rdtSocket.removePendingMessage(currentSteerMessageSeq);
            currentSteerMessageSeq = msgSeq.getAndIncrement();
            curDirection = direction;
            rdtSocket.sendNonBlocking(new SteerMessage(direction, currentSteerMessageSeq, playerID, masterID), master);
        }
    }

    @Override
    public void exit() {
        if (master != null) {
            rdtSocket.sendWithoutConfirm(
                    new RoleChangeMessage(NodeRole.VIEWER, NodeRole.MASTER, msgSeq.getAndIncrement(), playerID, masterID),
                    master
            );
        }
        nodeRole = NodeRole.VIEWER;
        rdtSocket.stop();
        timer.cancel();
        if (receiveThread != null) {
            receiveThread.interrupt();
            receiveThread = null;
        }
        stopCurrentServerGame();
    }

    @Override
    public void updateActiveGames(Set<GameInfo> gameInfos) {
        for (GameInfo game : gameInfos) {
            if (game.getMasterNode().getAddress().equals(rdtSocket.getAddress())) {
                anyGamesOnLocalhost = true;
                return;
            }
        }
        anyGamesOnLocalhost = false;
    }

    private void stopCurrentServerGame() {
        if (activeServerGame != null) {
            activeServerGame.stop();
        }
        activeServerGame = null;
        masterID = EMPTY;
        playerID = EMPTY;
        deputyID = EMPTY;
        master = null;
        deputy = null;
        gameState = null;
    }

    private void startTimerTasks() {
        timer.cancel();
        timer = new Timer();
        startSendPingMessages();
        startHandleReceivedMessages();
        startMasterCheck();
    }

    private void startHandleReceivedMessages() {
        receiveThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                MessageOwner recv = rdtSocket.receive();
                handleMessage(recv.getOwner(), recv.getMessage());
            }
        });
        receiveThread.start();
    }

    private void startSendPingMessages() {
        int pingDelay = config.getStateDelayMs() / 10;
        timer.schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        if (master != null) {
                            rdtSocket.sendWithoutConfirm(new PingMessage(msgSeq.getAndIncrement(), playerID, masterID), master);
                        }
                    }
                },
                0,
                pingDelay
        );
    }

    private void startMasterCheck() {
        int delay = (int) (config.getStateDelayMs() * 0.8);
        timer.schedule(
                new TimerTask() {
                    @Override
                    public void run() {
                        if (master != null && Duration.between(masterLastSeen, Instant.now()).abs().toMillis() > config.getStateDelayMs()) {
                            if (nodeRole == NodeRole.DEPUTY) {
                                swapToMaster();
                            }
                            else if (nodeRole == NodeRole.NORMAL) {
                                master = deputy;
                                deputy = null;
                                masterID = deputyID;
                                deputyID = EMPTY;
                                masterLastSeen = Instant.now();
                            }
                            if (curDirection != null) {
                                handleMove(curDirection);
                            }
                        }
                    }
                },
                0,
                delay
        );
    }

    private void handleMessage(NetNode sender, Message message) {
        switch (message.getType()) {
            case ROLE_CHANGE -> messageHandler.handle(sender, (RoleChangeMessage) message);
            case ERROR -> messageHandler.handle(sender, (ErrorMessage) message);
            case STATE -> messageHandler.handle(sender, (StateMessage) message);
            default -> throw new IllegalStateException("Can't handle this message type = " + message.getType());
        }
    }

    private void swapToMaster()  {
        try {
            activeServerGame = new ServerGame(gameState, multicastInfo, networkInterface, gameName);
            master = new NetNode(rdtSocket.getAddress(), activeServerGame.getPort());
            deputy = null;
            masterID = playerID;
            deputyID = EMPTY;
            changeNodeRole(NodeRole.MASTER);
            gameState.getActivePlayers().forEach(player -> {
                        if (player.getRole().equals(NodeRole.NORMAL)) {
                            rdtSocket.sendNonBlocking(
                                    new RoleChangeMessage(
                                            NodeRole.DEPUTY,
                                            NodeRole.NORMAL,
                                            msgSeq.getAndIncrement(),
                                            playerID,
                                            player.getId()
                                    ),
                                    player.getNetNode()
                            );
                        }
                    }
            );
        }
        catch (IOException exception) {
            logger.error(exception.getLocalizedMessage());
        }
    }

    private void changeNodeRole(NodeRole nodeRole) {
        if (config == null) {
            logger.error("Can't change role=" + this.nodeRole + " to " + nodeRole + " without config");
            exit();
            throw new IllegalStateException("Cant change role without config");
        }
        this.nodeRole = nodeRole;
        logger.info("Client: I am " + this.nodeRole);
    }

    private void lose() {
        System.out.println("Lose");
    }

    private MessageHandler createMessageHandler() {
        return new MessageHandler() {
            @Override
            public void handle(NetNode sender, DiscoverMessage message) {
                logger.error("Client shouldn't receive Discover messages");
            }

            @Override
            public void handle(NetNode sender, SteerMessage message) {
                logger.error("Client shouldn't receive Steer messages");
            }

            @Override
            public void handle(NetNode sender, JoinMessage message) {
                logger.error("Client shouldn't receive Join messages");
            }

            @Override
            public void handle(NetNode sender, PingMessage message) {
                logger.error("Client shouldn't receive Ping messages");
            }

            @Override
            public void handle(NetNode sender, StateMessage stateMsg) {
                if (master != null && !sender.equals(master)) {
                    logger.info("Received state from somewhere else: " + "MASTER=" + master + ", SENDER=" + sender);
                    return;
                }

                GameState newState = stateMsg.getGameState();
                if (gameState != null && gameState.getStateID() >= newState.getStateID()) {
                    logger.warn("Received state with id=" + newState.getStateID() + " less then last gamehandler state id=" + gameState.getStateID());
                    return;
                }

                master = sender;
                masterLastSeen = Instant.now();
                Player deputyPlayer = StateUtils.getDeputyFromState(newState);
                if (deputyPlayer == null) {
                    deputy = null;
                    deputyID = -1;
                }
                else {
                    deputy = deputyPlayer.getNetNode();
                    deputyID = deputyPlayer.getId();
                }
                gameState = newState;
                view.setMyPlayer(new NetNode(rdtSocket.getAddress(), rdtSocket.getLocalPort()));
                view.updateCurrentGame(newState);
            }

            @Override
            public void handle(NetNode sender, ErrorMessage message) {
                logger.error("ERROR=" + message.getErrorMessage());
            }

            @Override
            public void handle(NetNode sender, RoleChangeMessage roleChangeMsg) {
                switch (nodeRole) {
                    case DEPUTY -> {
                        if (roleChangeMsg.getSenderRole() == NodeRole.MASTER && roleChangeMsg.getReceiverRole() == NodeRole.MASTER) {
                            swapToMaster();
                            deputy = null;
                        }
                        else if (roleChangeMsg.getSenderRole() == NodeRole.MASTER &&
                                 roleChangeMsg.getReceiverRole() == NodeRole.VIEWER) {
                            lose();
                        }
                    }
                    case NORMAL -> {
                        if (roleChangeMsg.getSenderRole() == NodeRole.MASTER && roleChangeMsg.getReceiverRole() == NodeRole.DEPUTY) {
                            changeNodeRole(NodeRole.DEPUTY);
                            masterLastSeen = Instant.now();
                        }
                        else if (roleChangeMsg.getSenderRole() == NodeRole.DEPUTY &&
                                 roleChangeMsg.getReceiverRole() == NodeRole.NORMAL) {
                            master = deputy;
                            masterLastSeen = Instant.now();
                            deputy = null;
                        }
                        else if (roleChangeMsg.getSenderRole() == NodeRole.MASTER &&
                                 roleChangeMsg.getReceiverRole() == NodeRole.VIEWER) {
                            lose();
                        }
                    }
                    default -> logger.error("Received role change message: me=" + nodeRole +
                                            ", senderRole=" + roleChangeMsg.getSenderRole() +
                                            ", receiverRole=" + roleChangeMsg.getReceiverRole());
                }
            }
        };
    }
}
