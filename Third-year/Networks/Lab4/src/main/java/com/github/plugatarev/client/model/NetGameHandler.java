package com.github.plugatarev.client.model;

import com.github.plugatarev.SnakesProto.Direction;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.multicastreceiver.GameInfo;

import java.util.Set;

public interface NetGameHandler {
    void startNewGame();
    void joinGame(NetNode gameOwner, String playerName);
    void joinServerPlayer(NetNode serverNetNode);
    void handleMove(Direction direction);
    void exit();
    void updateActiveGames(Set<GameInfo> gameInfos);
}
