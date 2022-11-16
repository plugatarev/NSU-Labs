package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;

public final class PingMessage extends Message {
    public PingMessage(long messageSequence, int senderID, int receiverID) {
        super(MessageType.PING, messageSequence, senderID, receiverID);
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();
        builder.setPing(GameMessage.PingMsg.newBuilder().build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
