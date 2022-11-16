package com.github.plugatarev.client.controller.events;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@RequiredArgsConstructor
public abstract class UserEvent {
    @Getter private final EventType type;
}
