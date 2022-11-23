package com.github.plugatarev.multicastreceiver;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.Getter;
import com.github.plugatarev.gamehandler.Player;

import java.util.List;
import java.util.Objects;

public final class GameInfo {
    @Getter private final String gameName;
    @Getter private final GameConfig config;
    @Getter private final NetNode masterNode;
    @Getter private final List<Player> players;
    @Getter private final boolean canJoin;

    public GameInfo(String gameName, GameConfig config, NetNode masterNode, List<Player> players, boolean canJoin) {
        this.gameName = gameName;
        this.config = config;
        this.masterNode = masterNode;
        this.players = players;
        this.canJoin = canJoin;
    }

    @Override
    public boolean equals(Object object) {
        if (object == this) {
            return true;
        }
        if (object == null || getClass() != object.getClass()) {
            return false;
        }
        GameInfo other = (GameInfo) object;
        return gameName.equals(other.gameName) && (canJoin == other.canJoin) && (players == other.players) && config.equals(other.config);
    }

    @Override
    public int hashCode() {
        return Objects.hash(gameName, config, canJoin, players);
    }
}
