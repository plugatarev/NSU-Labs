package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.gamehandler.GameState;
import lombok.Getter;
import com.github.plugatarev.utils.StateUtils;

public final class StateMessage extends Message {
    @Getter private final GameState gameState;

    public StateMessage(GameState gameState, long messageSequence, int senderID, int receiverID) {
        super(MessageType.STATE, messageSequence, senderID, receiverID);
        this.gameState = gameState;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();
        var stateBuilder = GameMessage.StateMsg.newBuilder();
        stateBuilder.setState(StateUtils.createStateForMessage(gameState));
        builder.setState(stateBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
