package com.github.plugatarev.multicastreceiver;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.Getter;
import com.github.plugatarev.gamehandler.Player;

import javax.validation.constraints.NotNull;
import java.util.List;
import java.util.Objects;

public final class GameInfo {
    @Getter @NotNull private final GameConfig config;
    @Getter @NotNull private final NetNode masterNode;
    @Getter private final List<Player> players;
    @Getter private final boolean canJoin;

    @Getter @NotNull private String masterNodeName = "";

    public GameInfo(@NotNull GameConfig config, @NotNull NetNode masterNode, List<Player> players, boolean canJoin) {
        this.config = config;
        this.masterNode = masterNode;
        this.players = players;
        this.canJoin = canJoin;
        setMasterNodeName();
    }

    private void setMasterNodeName() {
        for (Player player : players) {
            if (player.getRole() == SnakesProto.NodeRole.MASTER) {
                masterNodeName = player.getName();
                return;
            }
        }
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (null == object || getClass() != object.getClass()) {
            return false;
        }
        GameInfo other = (GameInfo) object;
        return (canJoin == other.canJoin) && (players == other.players) && config.equals(other.config);
    }

    @Override
    public int hashCode() {
        return Objects.hash(config, canJoin, players);
    }
}
