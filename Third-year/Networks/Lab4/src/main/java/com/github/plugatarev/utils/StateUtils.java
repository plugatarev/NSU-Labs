package com.github.plugatarev.utils;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.config.ConfigProperty;
import com.github.plugatarev.gamehandler.GameState;
import com.github.plugatarev.gamehandler.Player;
import com.github.plugatarev.gamehandler.Coord;
import com.github.plugatarev.gamehandler.Snake;
import lombok.experimental.UtilityClass;
import org.apache.log4j.Logger;

@UtilityClass
public final class StateUtils {
    private static final Logger logger = Logger.getLogger(StateUtils.class);

    public static GameState getStateFromMessage(SnakesProto.GameState state) {
        if (!validateGameState(state)) {
            logger.info("Game state doesn't have required fields");
            return null;
        }
        return new GameState(
                PointUtils.getPointList(state.getFoodsList()),
                PlayerUtils.getPlayerList(state.getPlayers().getPlayersList()),
                SnakeUtils.getSnakeList(state.getSnakesList(), ConfigProperty.getConfig()),
                ConfigProperty.getConfig(),
                state.getStateOrder()
        );
    }

    private static boolean validateGameState(SnakesProto.GameState state) {
        return state.hasStateOrder() && state.hasPlayers();
    }

    public static SnakesProto.GameState createStateForMessage(GameState state) {
        var builder = SnakesProto.GameState.newBuilder();
        builder.setStateOrder(state.getStateID());
        for (Snake snake: state.getSnakes()) {
            builder.addSnakes(SnakeUtils.createSnakeForMessage(snake));
        }
        var coordBuilder = SnakesProto.GameState.Coord.newBuilder();
        for (Coord fruit: state.getFoods()) {
            coordBuilder.setX(fruit.getX());
            coordBuilder.setY(fruit.getY());
            builder.addFoods(coordBuilder.build());
        }
        var playersBuilder = SnakesProto.GamePlayers.newBuilder();
        for (Player player: state.getActivePlayers()) {
            playersBuilder.addPlayers(PlayerUtils.createPlayerForMessage(player));
        }
        builder.setPlayers(playersBuilder.build());
        return builder.build();
    }

    public static String getMasterNameFromState(GameState state) {
        for (Player player: state.getActivePlayers()) {
            if (SnakesProto.NodeRole.MASTER.equals(player.getRole())) {
                return player.getName();
            }
        }
        return "";
    }

    public static Player getDeputyFromState(GameState state) {
        for (Player player: state.getActivePlayers()) {
            if (SnakesProto.NodeRole.DEPUTY.equals(player.getRole())) {
                return player;
            }
        }
        return null;
    }
}
