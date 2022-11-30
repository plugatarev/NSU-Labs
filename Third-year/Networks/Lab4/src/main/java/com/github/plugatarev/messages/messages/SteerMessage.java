package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.Direction;
import com.github.plugatarev.SnakesProto.GameMessage;
import lombok.Getter;

public final class SteerMessage extends Message {
    @Getter private final Direction direction;

    public SteerMessage(Direction direction, long messageSequence, int senderID, int receiverID) {
        super(MessageType.STEER, messageSequence, senderID, receiverID);
        this.direction = direction;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();
        var steerBuilder = GameMessage.SteerMsg.newBuilder();
        steerBuilder.setDirection(direction);
        builder.setSteer(steerBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
