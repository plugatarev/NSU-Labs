package com.github.plugatarev.client.controller;

import com.github.plugatarev.client.controller.events.UserEvent;

import javax.validation.constraints.NotNull;

public interface GameController {
    void event(@NotNull UserEvent userEvent);
}
