package com.github.plugatarev.client.view.javafx;

import com.github.plugatarev.gamehandler.Player;
import javafx.scene.paint.Color;

import java.util.*;

public final class PlayerColorMapper {
    private final Map<Player, Color> playerColors;
    private int prevColorIndex;

    private static final List<Color> snakeColors = List.of(Color.RED, Color.BLUE, Color.PURPLE);

    private static final Color ZOMBIE_SNAKE_COLOR = Color.BLACK;

    public PlayerColorMapper() {
        playerColors = new HashMap<>();
        prevColorIndex = new Random().nextInt(snakeColors.size());
    }

    public Optional<Color> getColor(Player player) {
        return Optional.ofNullable(playerColors.get(player));
    }

    public void addPlayer(Player player) {
        int currentColorIndex = (prevColorIndex + 1) % snakeColors.size();
        playerColors.put(player, snakeColors.get(currentColorIndex));
        prevColorIndex = currentColorIndex;
    }

    public void removePlayer(Player player) {
        Objects.requireNonNull(player, "Player for remove cant be null");
        playerColors.remove(player);
    }

    public boolean isPlayerRegistered(Player player) {
        Objects.requireNonNull(player, "Player cant be null");
        return playerColors.containsKey(player);
    }

    public Set<Player> getRegisteredPlayers() {
        return Collections.unmodifiableSet(playerColors.keySet());
    }

    public Color getZombieSnakeColor() {
        return ZOMBIE_SNAKE_COLOR;
    }
}
