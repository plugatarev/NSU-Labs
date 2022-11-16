package com.github.plugatarev.client.controller.events;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.client.model.Game;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.Getter;

public final class JoinGameEvent extends UserEvent {
    @Getter private final NetNode node;
    @Getter private final GameConfig config;

    public JoinGameEvent(NetNode node, GameConfig config) {
        super(EventType.JOIN_GAME);
        this.node = node;
        this.config = config;
    }
}
