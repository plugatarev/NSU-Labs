package com.github.plugatarev.client.view;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.multicastreceiver.GameInfo;

import javax.validation.constraints.NotNull;
import java.util.Collection;

public interface View {
    void setConfig(@NotNull GameConfig gameConfig);
    void setMyPlayer(NetNode self);
    void updateCurrentGame(GameState state);
    void updateGameList(@NotNull Collection<GameInfo> gameInfos);
}
