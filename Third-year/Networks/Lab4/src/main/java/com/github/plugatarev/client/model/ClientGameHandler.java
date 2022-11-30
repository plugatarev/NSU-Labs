package com.github.plugatarev.client.model;

import com.github.plugatarev.SnakesProto.Direction;
import com.github.plugatarev.datatransfer.NetNode;

public interface ClientGameHandler {
    void startNewGame();
    void joinGame(NetNode gameOwner, String playerName);
    void joinServerPlayer(NetNode serverNetNode);
    void handleMove(Direction direction);
    void exit();
}
