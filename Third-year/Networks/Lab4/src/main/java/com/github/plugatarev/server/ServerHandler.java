package com.github.plugatarev.server;

import com.github.plugatarev.gamehandler.GameState;

public interface ServerHandler {
    void update(GameState state);
    int getPort();
    void stop();
}
