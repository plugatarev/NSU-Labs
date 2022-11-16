package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.NodeRole;
import com.github.plugatarev.SnakesProto.GameMessage;
import lombok.Getter;

public final class RoleChangeMessage extends Message {
    @Getter private final NodeRole senderRole;
    @Getter private final NodeRole receiverRole;

    public RoleChangeMessage(NodeRole senderRole, NodeRole receiverRole, long messageSequence, int senderID, int receiverID) {
        super(MessageType.ROLE_CHANGE, messageSequence, senderID, receiverID);
        this.senderRole = senderRole;
        this.receiverRole = receiverRole;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();

        var roleBuilder = GameMessage.RoleChangeMsg.newBuilder();
        roleBuilder.setSenderRole(senderRole);
        roleBuilder.setReceiverRole(receiverRole);

        builder.setRoleChange(roleBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        builder.setSenderId(getSenderID());
        builder.setReceiverId(getReceiverID());
        return builder.build();
    }
}
