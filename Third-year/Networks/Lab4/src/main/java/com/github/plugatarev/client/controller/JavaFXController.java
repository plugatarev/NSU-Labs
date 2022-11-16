package com.github.plugatarev.client.controller;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.client.controller.events.MoveEvent;
import com.github.plugatarev.client.controller.events.UserEvent;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.RequiredArgsConstructor;
import com.github.plugatarev.client.view.View;
import com.github.plugatarev.client.model.GameHandler;
import com.github.plugatarev.client.controller.events.JoinGameEvent;

@RequiredArgsConstructor
public final class JavaFXController implements GameController {
    private final GameConfig playerConfig;
    private final String playerName;
    private final GameHandler gameNetwork;
    private final View view;

    @Override
    public void event(UserEvent userEvent) {
        switch (userEvent.getType()) {
            case NEW_GAME -> {
                view.setConfig(playerConfig);
                gameNetwork.startNewGame();
            }
            case JOIN_GAME -> {
                JoinGameEvent joinEvent = (JoinGameEvent) userEvent;
                view.setConfig(joinEvent.getConfig());
                gameNetwork.joinGame(joinEvent.getNode(), playerName);
            }
            case MOVE -> {
                MoveEvent moveEvent = (MoveEvent) userEvent;
                gameNetwork.handleMove(moveEvent.getDirection());
            }
            case EXIT -> gameNetwork.exit();
        }
    }
}
