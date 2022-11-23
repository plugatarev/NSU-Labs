package com.github.plugatarev.utils;

import com.github.plugatarev.SnakesProto.GamePlayer;
import com.github.plugatarev.SnakesProto.PlayerType;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.Snake;
import lombok.experimental.UtilityClass;
import org.apache.log4j.Logger;

import java.net.UnknownHostException;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

@UtilityClass
public final class PlayerUtils {
    private static final Logger logger = Logger.getLogger(PlayerUtils.class);

    public static GamePlayer createPlayerForMessage(Player player) {
        GamePlayer.Builder builder = GamePlayer.newBuilder();
        builder.setName(player.getName());
        builder.setId(player.getId());
        builder.setIpAddress(player.getNetNode().getAddress().getHostAddress());
        builder.setPort(player.getNetNode().getPort());
        builder.setRole(player.getRole());
        builder.setType(PlayerType.HUMAN);
        builder.setScore(player.getScore());
        return builder.build();
    }

    public static Player findPlayerBySnake(Snake snake, List<Player> playerList) {
        for (Player player: playerList) {
            if (player.getId() == snake.getPlayerID()) {
                return player;
            }
        }
        return null;
    }

    public static Player findPlayerByAddress(NetNode node, Set<Player> players) {
        return players.stream().filter(player -> player.getNetNode().equals(node)).findFirst().orElse(null);
    }

    public static List<Player> getPlayerList(List<GamePlayer> gamePlayers) {
        return gamePlayers.stream().map(
                gamePlayer -> {
                    if (!validateGamePlayer(gamePlayer)) {
                        logger.error("Player doesn't have required fields");
                        return null;
                    }

                    Player player = null;
                    try {
                        NetNode node = new NetNode(gamePlayer.getIpAddress(), gamePlayer.getPort());
                        player = new Player(gamePlayer.getName(), gamePlayer.getId(), node);
                        player.setRole(gamePlayer.getRole());
                        player.setScore(gamePlayer.getScore());
                    }
                    catch (UnknownHostException e) {
                        logger.error(e.getLocalizedMessage());
                    }

                    return player;
                }
        ).filter(Objects::nonNull).collect(Collectors.toList());
    }

    private static boolean validateGamePlayer(GamePlayer gamePlayer) {
        return gamePlayer.hasName() &&
               gamePlayer.hasId() &&
               gamePlayer.hasIpAddress() &&
               gamePlayer.hasPort() &&
               gamePlayer.hasRole() &&
               gamePlayer.hasScore();
    }
}
