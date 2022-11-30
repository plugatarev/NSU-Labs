package com.github.plugatarev.master;

import com.github.plugatarev.gamehandler.GameState;

public interface MasterHandler {
    void update(GameState state);
    int getPort();
    void stop();
}
