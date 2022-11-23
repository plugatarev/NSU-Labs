package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.SnakesProto.NodeRole;
import com.github.plugatarev.SnakesProto.PlayerType;
import lombok.Getter;

public final class JoinMessage extends Message {
    private static final int EMPTY = -1;
    @Getter private final PlayerType playerType;
    @Getter private final String playerName;
    @Getter private final NodeRole role;
    @Getter private final String gameName;

    public JoinMessage(PlayerType playerType, String playerName, String gameName, NodeRole role, long messageSequence) {
        super(MessageType.JOIN, messageSequence, EMPTY, EMPTY);
        this.playerType = playerType;
        this.playerName = playerName;
        this.gameName = gameName;
        this.role = role;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();

        var joinBuilder = GameMessage.JoinMsg.newBuilder();
        joinBuilder.setPlayerName(playerName);
        joinBuilder.setGameName(gameName);
        joinBuilder.setRequestedRole(role);

        builder.setJoin(joinBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
