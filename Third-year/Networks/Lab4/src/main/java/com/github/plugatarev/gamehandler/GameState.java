package com.github.plugatarev.gamehandler;

import com.github.plugatarev.SnakesProto.GameConfig;
import lombok.Getter;
import lombok.RequiredArgsConstructor;

import java.io.Serializable;
import java.util.List;

@RequiredArgsConstructor
public final class GameState implements Serializable {
    @Getter private final List<Point2D> foods;
    @Getter private final List<Player> activePlayers;
    @Getter private final List<Snake> snakes;
    @Getter private final GameConfig gameConfig;
    @Getter private final int stateID;
}
