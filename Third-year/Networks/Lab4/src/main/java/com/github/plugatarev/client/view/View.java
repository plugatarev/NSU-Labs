package com.github.plugatarev.client.view;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.multicastreceiver.GameInfo;

import java.util.Collection;

public interface View {
    void setConfig(GameConfig gameConfig);
    void updateCurrentGame(GameState state);
    void updateGameList(Collection<GameInfo> gameInfos);
}
