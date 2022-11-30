package com.github.plugatarev.client.controller;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.client.controller.events.MoveEvent;
import com.github.plugatarev.client.controller.events.ServerPlayerEvent;
import com.github.plugatarev.client.controller.events.UserEvent;
import com.github.plugatarev.client.model.ClientGameHandler;
import lombok.RequiredArgsConstructor;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.client.controller.events.JoinGameEvent;

@RequiredArgsConstructor
public final class JavaFXController implements GameController {
    private final GameConfig config;
    private final String playerName;
    private final ClientGameHandler clientGameHandler;
    private final View view;

    @Override
    public void event(UserEvent userEvent) {
        switch (userEvent.getType()) {
            case NEW_GAME -> {
                view.setConfig(config);
                clientGameHandler.startNewGame();
            }
            case JOIN_GAME -> {
                JoinGameEvent joinEvent = (JoinGameEvent) userEvent;
                view.setConfig(joinEvent.getConfig());
                clientGameHandler.joinGame(joinEvent.getNode(), playerName);
            }
            case MOVE -> {
                MoveEvent moveEvent = (MoveEvent) userEvent;
                clientGameHandler.handleMove(moveEvent.getDirection());
            }
            case SERVER_PLAYER -> {
                ServerPlayerEvent serverPlayerEvent = (ServerPlayerEvent) userEvent;
                clientGameHandler.joinServerPlayer(serverPlayerEvent.getServerNetNode());
            }
            case EXIT -> clientGameHandler.exit();
        }
    }
}
