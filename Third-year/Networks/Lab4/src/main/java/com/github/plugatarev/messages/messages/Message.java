package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;
import lombok.Getter;
import lombok.RequiredArgsConstructor;


import java.io.Serializable;

@RequiredArgsConstructor
public abstract class Message implements Serializable {
    @Getter private final MessageType type;
    @Getter private final long messageSequence;
    @Getter private final int senderID;
    @Getter private final int receiverID;

    public abstract GameMessage getGameMessage();
}
