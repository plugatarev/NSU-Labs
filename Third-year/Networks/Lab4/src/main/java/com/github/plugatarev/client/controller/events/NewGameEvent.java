package com.github.plugatarev.client.controller.events;

public final class NewGameEvent extends UserEvent {
    public NewGameEvent() {
        super(EventType.NEW_GAME);
    }
}
