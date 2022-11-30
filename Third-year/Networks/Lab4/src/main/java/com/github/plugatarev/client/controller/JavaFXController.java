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
    private final ClientGameHandler clientGame;
    private final View view;

    @Override
    public void event(UserEvent userEvent) {
        switch (userEvent.getType()) {
            case NEW_GAME -> {
                view.setConfig(config);
                clientGame.startNewGame();
            }
            case JOIN_GAME -> {
                JoinGameEvent joinEvent = (JoinGameEvent) userEvent;
                view.setConfig(joinEvent.getConfig());
                clientGame.joinGame(joinEvent.getNode(), playerName);
            }
            case MOVE -> {
                MoveEvent moveEvent = (MoveEvent) userEvent;
                clientGame.handleMove(moveEvent.getDirection());
            }
            case SERVER_PLAYER -> {
                ServerPlayerEvent serverPlayerEvent = (ServerPlayerEvent) userEvent;
                clientGame.joinServerPlayer(serverPlayerEvent.getServerNetNode());
            }
            case EXIT -> clientGame.exit();
        }
    }
}
