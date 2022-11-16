package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;

public final class AckMessage extends Message {
    public AckMessage(long messageSequence, int senderID, int receiverID) {
        super(MessageType.ACK, messageSequence, senderID, receiverID);
    }

    @Override
    public GameMessage getGameMessage() {
        GameMessage.Builder builder = GameMessage.newBuilder();
        builder.setAck(GameMessage.AckMsg.newBuilder().build());
        builder.setMsgSeq(getMessageSequence());
        builder.setSenderId(getSenderID());
        builder.setReceiverId(getReceiverID());
        return builder.build();
    }
}
