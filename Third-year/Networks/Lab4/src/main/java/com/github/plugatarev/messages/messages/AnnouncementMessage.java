package com.github.plugatarev.messages.messages;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.GameAnnouncement;
import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.SnakesProto.GamePlayers;
import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.gamehandler.Player;
import lombok.AccessLevel;
import lombok.Getter;
import com.github.plugatarev.utils.PlayerUtils;

import java.util.ArrayList;
import java.util.List;

public final class AnnouncementMessage extends Message {
    private static final int EMPTY = -1;

    @Getter private final List<GameAnnouncement> games;

    public AnnouncementMessage(List<GameAnnouncement> games, long messageSequence) {
        super(MessageType.ANNOUNCEMENT, messageSequence, EMPTY, EMPTY);
        this.games = games;
    }

    public AnnouncementMessage(long messageSequence) {
        super(MessageType.ANNOUNCEMENT, messageSequence, EMPTY, EMPTY);
        this.games = new ArrayList<>();
    }

    public void addGame(List<Player> players, GameConfig config, String gameName, boolean canJoin) {
        GameAnnouncement.Builder gameAnnouncement = GameAnnouncement.newBuilder();
        gameAnnouncement.setGameName(gameName);
        var gamePlayersBuilder = SnakesProto.GamePlayers.newBuilder();
        for (var player : players) {
            gamePlayersBuilder.addPlayers(PlayerUtils.createPlayerForMessage(player));
        }
        gameAnnouncement.setPlayers(gamePlayersBuilder.build());
        gameAnnouncement.setConfig(config);
        gameAnnouncement.setCanJoin(canJoin);
        games.add(gameAnnouncement.build());
    }

    @Override
    public GameMessage getGameMessage() {
        var builder = GameMessage.newBuilder();
        GameMessage.AnnouncementMsg.Builder announcementBuilder = GameMessage.AnnouncementMsg.newBuilder();
        for (GameAnnouncement game : games) {
            announcementBuilder.addGames(game);
        }
        builder.setAnnouncement(announcementBuilder.build());
        builder.setMsgSeq(getMessageSequence());
        return builder.build();
    }
}
