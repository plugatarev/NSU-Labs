package com.github.plugatarev.gamehandler.game;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.Snake;

import java.util.Map;

public interface GameHandler {
    Player registerNewPlayer(String playerName, NetNode netNode);
    void removePlayer(Player player);
    void moveAllSnakes(Map<Player, SnakesProto.Direction> playersMoves);
    Snake getSnakeByPlayer(Player player);
}
