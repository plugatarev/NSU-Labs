package com.github.plugatarev.client.view.javafx;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.SnakesProto.Direction;
import com.github.plugatarev.client.controller.GameController;
import com.github.plugatarev.client.controller.events.*;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.Snake;
import com.github.plugatarev.multicastreceiver.GameInfo;
import com.github.plugatarev.utils.PlayerUtils;
import com.github.plugatarev.utils.StateUtils;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import org.apache.log4j.Logger;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.*;

public final class GameView implements View {
    private static final Logger logger = Logger.getLogger(GameView.class);

    private static final Paint FOOD_COLOR = Color.GREEN;
    private static final Paint EMPTY_CELL_COLOR = Color.WHITE;

    private @FXML TableColumn<ActiveGameButton, String> gameNameColumn;
    private @FXML TableColumn<ActiveGameButton, Integer> playersNumberColumn;
    private @FXML TableColumn<ActiveGameButton, String> fieldSizeColumn;
    private @FXML TableColumn<ActiveGameButton, String> foodColumn;
    private @FXML TableColumn<ActiveGameButton, Button> connectButtonColumn;
    private @FXML TableColumn<Player, String> playerNameColumn;
    private @FXML TableColumn<Player, Integer> playerScoreColumn;
    private @FXML Label gameOwner;
    private @FXML Label foodAmount;
    private @FXML Label fieldSize;
    private @FXML TableView<Player> playersRankingTable;
    private @FXML Button exitButton;
    private @FXML Button serverPlayer;
    private @FXML Button newGameButton;
    private @FXML TableView<ActiveGameButton> gameListTable;
    private @FXML BorderPane gameFieldPane;
    private @FXML TextField serverPort;
    private @FXML TextField serverName;

    private final ObservableList<Player> playersObservableList = FXCollections.observableArrayList();
    private final ObservableList<ActiveGameButton> gameInfoObservableList = FXCollections.observableArrayList();
    private final Set<ActiveGameButton> activeGameButtons = new HashSet<>();
    private final PlayerColorMapper colorMapper = new PlayerColorMapper();

    private Rectangle[][] fieldCells;
    private Stage stage;
    private GameConfig gameConfig;
    private GameController gameController;

    private String serverIPName;
    private String serverPortName;

    public void setGameController(GameController controller) {
        this.gameController = controller;
    }

    public void setStage(Stage stage) {
        this.stage = stage;
        this.stage.setMaximized(true);
        this.stage.setOnCloseRequest(event -> close(true));
        this.stage.addEventHandler(KeyEvent.KEY_RELEASED, event -> {
            if (gameController == null) {
                throw new IllegalStateException("Can't move with undefined controller");
            }
            getDirectionByKeyCode(event.getCode()).ifPresent(direction -> gameController.event(new MoveEvent(direction)));
        });
        initPlayersInfoTable();
        initGameListTable();
        setActionOnButtons();
    }

    @Override
    public void updateCurrentGame(GameState state) {
        Platform.runLater(() -> {
            foodAmount.setText(String.valueOf(state.getFoods().size()));
            fieldSize.setText(state.getGameConfig().getHeight() + "x" + state.getGameConfig().getWidth());
            gameOwner.setText(StateUtils.getMasterNameFromState(state));
        });
        playersObservableList.setAll(state.getActivePlayers());
        updateField(state);
    }

    @Override
    public void setConfig(GameConfig gameConfig) {
        this.gameConfig = gameConfig;
        buildField();
    }

    @Override
    public void updateGameList(Collection<GameInfo> gameInfos) {
        activeGameButtons.clear();
        gameInfos.forEach(gameInfo -> {
            ActiveGameButton activeGameButton = new ActiveGameButton(gameInfo);
            activeGameButtons.add(activeGameButton);

            Button button = activeGameButton.getButton();
            button.setOnAction(event ->
                    gameController.event(
                            new JoinGameEvent(activeGameButton.getMasterNode(), activeGameButton.getConfig())
                    )
            );
        });
        gameInfoObservableList.setAll(activeGameButtons);
    }

    private void initPlayersInfoTable() {
        playersRankingTable.setItems(playersObservableList);
        playerNameColumn.setCellValueFactory(new PropertyValueFactory<>("name"));
        playerScoreColumn.setCellValueFactory(new PropertyValueFactory<>("score"));
    }

    private void initGameListTable() {
        gameListTable.setItems(gameInfoObservableList);
        gameNameColumn.setCellValueFactory(new PropertyValueFactory<>("gameName"));
        foodColumn.setCellValueFactory(new PropertyValueFactory<>("foodNumber"));
        playersNumberColumn.setCellValueFactory(new PropertyValueFactory<>("playersCount"));
        fieldSizeColumn.setCellValueFactory(new PropertyValueFactory<>("fieldSize"));
        connectButtonColumn.setCellValueFactory(new PropertyValueFactory<>("button"));
    }

    private void setActionOnButtons() {
        exitButton.setOnAction(event -> close(false));
        newGameButton.setOnAction(event -> gameController.event(new NewGameEvent()));

        serverName.setOnAction(event -> serverIPName = serverName.getText());
        serverPort.setOnAction(event -> serverPortName = serverPort.getText());

        serverPlayer.setOnAction(event -> {
                try {
                    if (serverIPName != null && serverPortName != null) {
                        NetNode node = new NetNode(InetAddress.getByName(serverIPName), Integer.parseInt(serverPortName));
                        gameController.event(new ServerPlayerEvent(node));
                    } else {
                        gameController.event(new ServerPlayerEvent());
                    }
                }
                catch (UnknownHostException e) {
                    logger.error("Server not found. Exception = " + e.getMessage());
                }
            }
        );
    }

    private void close(boolean closeStage) {
        if (closeStage) {
            if (stage == null) {
                throw new IllegalStateException("Cant close uninitialized stage");
            }
            stage.close();
        }
        gameController.event(new ExitEvent());


        Platform.runLater(() -> {
            foodAmount.setText("");
            fieldSize.setText("");
            gameOwner.setText("");
        });
        playersObservableList.clear();

        if (gameConfig != null && fieldCells != null) {
            for (int row = 0; row < gameConfig.getHeight(); row++) {
                for (int col = 0; col < gameConfig.getWidth(); col++) {
                    fieldCells[row][col].setFill(EMPTY_CELL_COLOR);
                }
            }
        }
    }

    private Optional<Direction> getDirectionByKeyCode(KeyCode code) {
        return switch (code) {
            case UP, W -> Optional.of(Direction.UP);
            case DOWN, S -> Optional.of(Direction.DOWN);
            case RIGHT, D -> Optional.of(Direction.RIGHT);
            case LEFT, A -> Optional.of(Direction.LEFT);
            default -> Optional.empty();
        };
    }

    private void updateField(GameState state) {
        Map<Snake, Color> snakes = createSnakesMap(state);
        for (int row = 0; row < gameConfig.getHeight(); row++) {
            for (int col = 0; col < gameConfig.getWidth(); col++) {
                fieldCells[row][col].setFill(EMPTY_CELL_COLOR);
            }
        }
        snakes.forEach((snake, color) ->
                snake.getCoordinates().forEach(point -> {
                    Color pointColor = snake.isSnakeHead(point) ?
                            ((color.darker().equals(color)) ?
                                    color.brighter() :
                                    color.darker()) :
                            color;
                    fieldCells[point.y()][point.x()].setFill(pointColor);
                })
        );
        state.getFoods().forEach(fruit -> fieldCells[fruit.y()][fruit.x()].setFill(FOOD_COLOR));
    }

    private void buildField() {
        int gameFieldHeight = gameConfig.getHeight();
        int gameFieldWidth = gameConfig.getWidth();
        int rectHeight = (int) (gameFieldPane.getPrefHeight() / gameFieldHeight);
        int rectWidth = (int) (gameFieldPane.getPrefWidth() / gameFieldWidth);
        GridPane gridPane = new GridPane();
        fieldCells = new Rectangle[gameFieldHeight][gameFieldWidth];
        for (int row = 0; row < gameFieldHeight; row++) {
            for (int col = 0; col < gameFieldWidth; col++) {
                Rectangle rectangle = new Rectangle(rectWidth, rectHeight, EMPTY_CELL_COLOR);
                fieldCells[row][col] = rectangle;
                gridPane.add(rectangle, col, row);
            }
        }
        gridPane.setGridLinesVisible(true);
        gameFieldPane.setCenter(gridPane);
    }

    private Map<Snake, Color> createSnakesMap(GameState state) {
        updatePlayersColors(state.getActivePlayers());
        Map<Snake, Color> snakes = new HashMap<>();
        for (var snake : state.getSnakes()) {
            if (snake.getState() == SnakesProto.GameState.Snake.SnakeState.ZOMBIE) {
                snakes.put(snake, colorMapper.getZombieColor());
                continue;
            }
            Color playerColor = colorMapper.getColor(
                        Optional.ofNullable(PlayerUtils.findPlayerBySnake(snake, state.getActivePlayers())).orElseThrow()
                    ).orElseThrow(() -> new NoSuchElementException("Color map doesn't contain player"));
            snakes.put(snake, playerColor);
        }
        return snakes;
    }

    private void updatePlayersColors(List<Player> players) {
        removeInactivePlayersFromColorMap(players);
        players.forEach(activePlayer -> {
            if (!colorMapper.isPlayerRegistered(activePlayer)) {
                colorMapper.addPlayer(activePlayer);
            }
        });
    }

    private void removeInactivePlayersFromColorMap(List<Player> players) {
        List<Player> inactiveRegisteredUsers = colorMapper.getRegisteredPlayers()
                                                               .stream()
                                                               .filter(registeredPlayer -> !players.contains(registeredPlayer))
                                                               .toList();
        inactiveRegisteredUsers.forEach(colorMapper::removePlayer);
    }
}
