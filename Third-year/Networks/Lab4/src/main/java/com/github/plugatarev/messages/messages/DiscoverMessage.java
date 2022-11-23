package com.github.plugatarev.messages.messages;
import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.SnakesProto;

public class DiscoverMessage extends Message {
    public DiscoverMessage(long messageSequence, int senderID, int receiverID) {
        super(MessageType.DISCOVER, messageSequence, senderID, receiverID);
    }

    @Override
    public SnakesProto.GameMessage getGameMessage() {
        var build = GameMessage.newBuilder();

        var discoverBuilder = GameMessage.DiscoverMsg.newBuilder();
        build.setDiscover(discoverBuilder.build());

        return build.build();
    }
}
