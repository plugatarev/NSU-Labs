package com.github.plugatarev.gamehandler.game;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.GameHandler;
import lombok.Getter;
import org.apache.log4j.Logger;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.Point2D;
import com.github.plugatarev.gamehandler.Snake;
import com.github.plugatarev.server.ServerHandler;
import com.github.plugatarev.utils.PlayerUtils;

import javax.validation.constraints.NotNull;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public final class Game implements GameHandler {
    private static final Logger logger = Logger.getLogger(Game.class);

    private static final int SIZE_OF_EMPTY_SQUARE_FOR_SNAKE = 5;
    private static final String UNKNOWN_PLAYER_ERROR_MESSAGE = "Unknown player";
    private static final double PROBABILITY = 0.5;

    @Getter private final Map<Player, Snake> playersWithSnakes = new HashMap<>();
    private final Map<Player, Snake> playersForRemove = new HashMap<>();
    private final List<Snake> zombieSnakes = new ArrayList<>();
    private final List<Player> players = new ArrayList<>();
    private final Random random = new Random();

    @Getter private final GameConfig config;
    private final GameField field;
    private final List<Cell> foods;
    private final ServerHandler serverHandler;

    private int stateID;
    private int playerIDCounter = 1;

    public Game(GameConfig config, ServerHandler serverHandler) {
        this.config = config;
        this.serverHandler = serverHandler;
        this.field = new GameField(this.config.getWidth(), this.config.getHeight());
        this.stateID = 0;
        this.foods = new ArrayList<>(this.config.getFoodStatic());
        this.generateFoods();
    }

    public Game(GameState state, ServerHandler serverHandler) {
        this.config = state.getGameConfig();
        this.serverHandler = serverHandler;
        this.field = new GameField(this.config.getWidth(), this.config.getHeight());
        this.stateID = state.getStateID() + 1;

        state.getSnakes().forEach(snake -> {
            snake.getPoints().forEach(point -> this.field.set(point, CellType.SNAKE));
            switch (snake.getState()) {
                case ZOMBIE -> this.zombieSnakes.add(snake);
                case ALIVE -> {
                    Player snakeOwner = Optional.ofNullable(PlayerUtils.findPlayerBySnake(snake, state.getActivePlayers()))
                            .orElseThrow(() -> new IllegalStateException("Cant get player from alive snake"));
                    this.playersWithSnakes.put(snakeOwner, snake);
                }
            }
        });

        for (var player : state.getActivePlayers()) {
            this.players.add(player);
            if (player.getId() >= this.playerIDCounter) {
                this.playerIDCounter = player.getId() + 1;
            }
        }

        foods = new ArrayList<>(state.getFoods().size());
        state.getFoods().forEach(fruit -> {
            field.set(fruit, CellType.FOOD);
            foods.add(new Cell(fruit, CellType.FOOD));
        });
    }

    @Override
    public Player registerNewPlayer(@NotNull String playerName, NetNode netNode) {
        Player player = new Player(playerName, playerIDCounter, netNode);
        playerIDCounter++;

        List<Cell> snakePoints = createNewSnake();
        if (snakePoints.isEmpty()) {
            throw new IllegalStateException("Cant add new player because no space on field");
        }

        Snake playerSnake = new Snake(
                snakePoints.get(0).getPoint(),
                snakePoints.get(1).getPoint(),
                this.field.getWidth(),
                this.field.getHeight()
        );

        playerSnake.setPlayerID(player.getId());
        snakePoints.forEach(cell -> this.field.set(cell.getY(), cell.getX(), CellType.SNAKE));

        this.players.add(player);
        this.playersWithSnakes.put(player, playerSnake);
        return player;
    }

    @Override
    public void removePlayer(Player player) {
        if (this.playersWithSnakes.containsKey(player)) {
            Snake snake = this.playersWithSnakes.get(player);
            snake.setState(SnakesProto.GameState.Snake.SnakeState.ZOMBIE);
            this.zombieSnakes.add(snake);
            markPlayerInactive(player);
        }
    }

    @Override
    public void moveAllSnakes(Map<Player, SnakesProto.Direction> playersMoves) {
        this.playersWithSnakes.keySet().forEach(player -> makeMove(player, playersMoves.getOrDefault(player, null)));
        zombieSnakesMove();
        generateFoods();

        this.playersForRemove.keySet().forEach(player -> {
            makeFoodsFromSnakeWithProbability(this.playersWithSnakes.get(player));
            markPlayerInactive(player);
        });
        this.playersForRemove.clear();
        this.serverHandler.update(generateGameState());
    }

    @Override
    public Snake getSnakeByPlayer(Player player) {
        return this.playersWithSnakes.get(player);
    }

    private List<Cell> createNewSnake() {
        Optional<Cell> centerOfEmptySquareOnField = field.findCenterOfSquareWithOutSnake(SIZE_OF_EMPTY_SQUARE_FOR_SNAKE);
        if (centerOfEmptySquareOnField.isEmpty()) {
            return Collections.emptyList();
        }

        Cell head = centerOfEmptySquareOnField.get();
        Optional<Cell> tail =
                Stream.of(
                        field.get(head.getY() - 1, head.getX()),
                        field.get(head.getY() + 1, head.getX()),
                        field.get(head.getY(), head.getX() - 1),
                        field.get(head.getY(), head.getX() + 1)
                )
                .filter(cell -> CellType.EMPTY.equals(cell.getType()))
                .findFirst();
        if (tail.isEmpty()) {
            return Collections.emptyList();
        }

        return List.of(head, tail.get());
    }

    private void markPlayerInactive(Player player) {
        this.playersWithSnakes.remove(player);
        this.players.remove(player);
    }

    private void makeMove(Player player, SnakesProto.Direction direction) {
        if (!this.playersWithSnakes.containsKey(player)) {
            throw new IllegalArgumentException(UNKNOWN_PLAYER_ERROR_MESSAGE);
        }

        Snake snake = this.playersWithSnakes.get(player);
        snake.makeMove(direction);

        if (isSnakeCrashed(snake)) {
            handlePlayerLose(player, snake);
            return;
        }
        if (isSnakeAteFood(snake)) {
            incrementScore(player);
            removeFood(snake.getHead());
        }
        else {
            this.field.set(snake.getTail(), CellType.EMPTY);
            snake.removeTail();
        }
        this.field.set(snake.getHead(), CellType.SNAKE);
    }

    private void removeFood(Point2D fruitForRemove) {
        this.foods.removeIf(fruit -> fruitForRemove.equals(fruit.getPoint()));
    }

    private void handlePlayerLose(Player player, Snake playerSnake) {
        this.playersForRemove.put(player, playerSnake);
    }

    private void zombieSnakesMove() {
        this.zombieSnakes.forEach(this::zombieMove);
        this.zombieSnakes.stream().filter(this::isSnakeCrashed).forEach(this::makeFoodsFromSnakeWithProbability);
        this.zombieSnakes.removeIf(this::isSnakeCrashed);
    }

    private void zombieMove(Snake snake) {
        snake.makeMove();
        if (isSnakeAteFood(snake)) {
            removeFood(snake.getHead());
        }
        else {
            this.field.set(snake.getTail(), CellType.EMPTY);
            snake.removeTail();
        }
        this.field.set(snake.getHead(), CellType.SNAKE);
    }

    private void generateFoods() {
        int aliveSnakesCount = this.playersWithSnakes.size();
        //TODO: new task
        //int requiredFruitsNumber = this.config.getFoodStatic() + (int) (this.config.getFoodPerPlayer() * aliveSnakesCount);
        int requiredFruitsNumber = config.getFoodStatic() + aliveSnakesCount;
        if (foods.size() == requiredFruitsNumber) {
            return;
        }
        if (field.getEmptyCellsNumber() < requiredFruitsNumber) {
            logger.debug("Cant generate required number of fruits=" + requiredFruitsNumber + ", empty cells number=" + field.getEmptyCellsNumber());
            return;
        }

        while (foods.size() < requiredFruitsNumber) {
            Cell randomEmptyCell = field.findRandomEmptyCell().orElseThrow(() -> new IllegalStateException("Cant find empty cell"));
            field.set(randomEmptyCell.getPoint(), CellType.FOOD);
            foods.add(randomEmptyCell);
        }
    }

    private void incrementScore(Player player) {
        if (!players.contains(player)) {
            throw new IllegalArgumentException(UNKNOWN_PLAYER_ERROR_MESSAGE);
        }
        player.incrementScore();
    }

    private boolean isSnakeAteFood(Snake snake) {
        Point2D snakeHead = snake.getHead();
        return foods.stream().anyMatch(fruit -> snakeHead.equals(fruit.getPoint()));
    }

    private void makeFoodsFromSnakeWithProbability(Snake snake) {
        for (Point2D p : snake.getPoints()) {
            if (p.equals(snake.getHead())) {
                continue;
            }
            if (random.nextDouble() < PROBABILITY) {
                field.set(p, CellType.FOOD);
                foods.add(field.get(p.getY(), p.getX()));
            }
            else {
                field.set(p, CellType.EMPTY);
            }
        }
    }

    private boolean isSnakeCrashed(Snake snake) {
        if (isSnakeCrashedToZombie(snake)) {
            return true;
        }
        for (Map.Entry<Player, Snake> playerWithSnake : this.playersWithSnakes.entrySet()) {
            Snake otherSnake = playerWithSnake.getValue();
            if (checkCrashIntoYourself(snake)) {
                return true;
            }
            if (otherSnake != snake && otherSnake.isSnake(snake.getHead())) {
                incrementScore(playerWithSnake.getKey());
                return true;
            }
        }
        return false;
    }

    private boolean isSnakeCrashedToZombie(Snake snake) {
        return this.zombieSnakes.stream().anyMatch(zombieSnake -> zombieSnake != snake && zombieSnake.isSnake(snake.getHead()));
    }

    private boolean checkCrashIntoYourself(Snake snake) {
        return snake.isSnakeBody(snake.getHead()) || snake.getTail().equals(snake.getHead());
    }

    private GameState generateGameState() {
        int currentStateID = this.stateID++;
        return new GameState(getFoodsAsPointsList(), new ArrayList<>(this.players), generateSnakeList(), this.config, currentStateID);
    }

    private List<Point2D> getFoodsAsPointsList() {
        return this.foods.stream().map(Cell::getPoint).collect(Collectors.toList());
    }

    private List<Snake> generateSnakeList() {
        List<Snake> snakes = new ArrayList<>(this.playersWithSnakes.size() + this.zombieSnakes.size());
        this.playersWithSnakes.forEach((player, snake) -> snakes.add(snake));
        snakes.addAll(this.zombieSnakes);
        return snakes;
    }
}
