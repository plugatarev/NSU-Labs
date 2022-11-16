package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;
import lombok.Getter;

public final class ErrorMessage extends Message {
    @Getter private final String errorMessage;

    public ErrorMessage(String errorMessage, long messageSequence, int senderID, int receiverID) {
        super(MessageType.ERROR, messageSequence, senderID, receiverID);
        this.errorMessage = errorMessage;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();

        var errorBuilder = GameMessage.ErrorMsg.newBuilder();
        errorBuilder.setErrorMessage(errorMessage);

        builder.setError(errorBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
