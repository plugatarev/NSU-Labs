package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.Getter;

public final class JoinMessage extends Message {
    private static final int EMPTY = -1;
    @Getter private final NetNode nodeMaster;
    @Getter private final String playerName;
    @Getter private final SnakesProto.NodeRole role;
    @Getter private final String gameName;

    public JoinMessage(NetNode nodeMaster, String playerName, SnakesProto.NodeRole role, String gameName, long messageSequence) {
        super(MessageType.JOIN, messageSequence, EMPTY, EMPTY);
        this.nodeMaster = nodeMaster;
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
