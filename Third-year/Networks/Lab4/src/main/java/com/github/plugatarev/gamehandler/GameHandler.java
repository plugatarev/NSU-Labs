package com.github.plugatarev.gamehandler;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.datatransfer.NetNode;

import javax.validation.constraints.NotNull;
import java.util.Map;

public interface GameHandler {
    Player registerNewPlayer(String playerName, NetNode netNode);
    void removePlayer(Player player);
    void moveAllSnakes(Map<Player, SnakesProto.Direction> playersMoves);
    Snake getSnakeByPlayer(Player player);
}
