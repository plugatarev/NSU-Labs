package com.github.plugatarev.client.controller;

import com.github.plugatarev.client.controller.events.UserEvent;

public interface GameController {
    void event(UserEvent userEvent);
}
