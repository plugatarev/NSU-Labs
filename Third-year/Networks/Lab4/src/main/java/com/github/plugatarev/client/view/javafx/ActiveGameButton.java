package com.github.plugatarev.client.view.javafx;

import com.github.plugatarev.SnakesProto.GameConfig;
import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.multicastreceiver.GameInfo;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.CornerRadii;
import javafx.scene.paint.Color;
import lombok.Getter;

import javax.validation.constraints.NotNull;
import java.util.Objects;

public final class ActiveGameButton {
    @Getter private final String gameName;
    @Getter private final NetNode masterNode;
    @Getter private final int playersNumber;

    @Getter private final GameConfig config;
//    private final boolean canJoin;

    @Getter private final String fieldSize;
    @Getter private final String foodNumber;

    @Getter private final Button button;

    public ActiveGameButton(@NotNull GameInfo gameInfo) {
        Objects.requireNonNull(gameInfo); //TODO: mb delete it?
        gameName = gameInfo.getGameName();
        playersNumber = gameInfo.getPlayers().size();
        config = gameInfo.getConfig();
        masterNode = gameInfo.getMasterNode();
//        canJoin = gameInfo.isCanJoin();
        button = new Button("Вход");
        fieldSize = config.getHeight() + "x" + config.getWidth();
        foodNumber = config.getFoodStatic() + ": x" + config.getFoodStatic();
        designButton();
    }

    private void designButton() {
        button.setBackground(new Background(new BackgroundFill(Color.BLUE, new CornerRadii(0), new Insets(0))));
        button.setTextFill(Color.WHITE);
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (!(object instanceof ActiveGameButton other)) {
            return false;
        }
        return gameName.equals(other.gameName) && masterNode.equals(other.masterNode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(masterNode, gameName);
    }
}
