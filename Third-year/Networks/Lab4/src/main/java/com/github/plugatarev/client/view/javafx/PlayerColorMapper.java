package com.github.plugatarev.client.view.javafx;

import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.Player;
import javafx.scene.paint.Color;

import javax.validation.constraints.NotNull;
import java.util.*;

public final class PlayerColorMapper {
    @NotNull private final Map<Player, Color> playerColors;
    private int prevColorIndex;

    private static final List<Color> snakeColors = List.of(Color.RED, Color.ORANGE, Color.PURPLE);

    private static final Color ZOMBIE_SNAKE_COLOR = Color.BLACK;
    private static final Color MY_SNAKE_COLOR = Color.BLUE;

    public PlayerColorMapper() {
        playerColors = new HashMap<>();
        prevColorIndex = new Random().nextInt(snakeColors.size());
    }

    @NotNull
    public Optional<Color> getColor(@NotNull Player player, NetNode self) {
        if (self != null) {
            if (player.getNetNode().equals(self)) {
                return Optional.of(MY_SNAKE_COLOR);
            }
        }
        return Optional.ofNullable(playerColors.get(player));
    }

    public void addPlayer(@NotNull Player player) {
        int currentColorIndex = (prevColorIndex + 1) % snakeColors.size();
        playerColors.put(Objects.requireNonNull(player), snakeColors.get(currentColorIndex));
        prevColorIndex = currentColorIndex;
    }

    public void removePlayer(@NotNull Player player) {
        Objects.requireNonNull(player, "Player for remove cant be null");
        playerColors.remove(player);
    }

    public boolean isPlayerRegistered(@NotNull Player player) {
        Objects.requireNonNull(player, "Player cant be null");
        return playerColors.containsKey(player);
    }

    @NotNull
    public Set<Player> getRegisteredPlayers() {
        return Collections.unmodifiableSet(playerColors.keySet());
    }

    @NotNull
    public Color getZombieSnakeColor() {
        return ZOMBIE_SNAKE_COLOR;
    }
}
