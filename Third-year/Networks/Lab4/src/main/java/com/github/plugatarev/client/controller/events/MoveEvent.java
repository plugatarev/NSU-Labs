package com.github.plugatarev.client.controller.events;

import com.github.plugatarev.SnakesProto.Direction;
import lombok.Getter;

public final class MoveEvent extends UserEvent {
    @Getter private final Direction direction;

    public MoveEvent(Direction direction) {
        super(EventType.MOVE);
        this.direction = direction;
    }
}
