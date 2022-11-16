package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.SnakesProto.GamePlayers;
import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.gamehandler.Player;
import lombok.Getter;
import com.github.plugatarev.utils.PlayerUtils;

import java.util.List;

public final class AnnouncementMessage extends Message {
    private static final int EMPTY = -1;

    @Getter private final List<Player> players;
    @Getter private final GameConfig config;
    @Getter private final boolean canJoin;
    @Getter private final String gameName;

    public AnnouncementMessage(GameConfig config, List<Player> players, boolean canJoin, String gameName, long messageSequence) {
        super(MessageType.ANNOUNCEMENT, messageSequence, EMPTY, EMPTY);
        this.players = players;
        this.config = config;
        this.canJoin = canJoin;
        this.gameName = gameName;
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();

        GamePlayers.Builder gamePlayersBuilder = GamePlayers.newBuilder(); //GamePlayers
        players.stream().map(PlayerUtils::createPlayerForMessage).forEach(gamePlayersBuilder::addPlayers);

        SnakesProto.GameAnnouncement.Builder gameAnnouncementBuilder = SnakesProto.GameAnnouncement.newBuilder();
        gameAnnouncementBuilder.setCanJoin(canJoin);
        gameAnnouncementBuilder.setConfig(config);
        gameAnnouncementBuilder.setGameName(gameName);
        gameAnnouncementBuilder.setPlayers(gamePlayersBuilder.build());

        GameMessage.AnnouncementMsg.Builder announcementBuilder = GameMessage.AnnouncementMsg.newBuilder();
        announcementBuilder.addGames(gameAnnouncementBuilder.build());

        builder.setAnnouncement(announcementBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
