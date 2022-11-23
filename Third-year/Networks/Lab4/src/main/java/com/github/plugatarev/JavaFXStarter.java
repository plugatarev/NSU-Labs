package com.github.plugatarev;

import com.github.plugatarev.client.controller.JavaFXController;
import com.github.plugatarev.client.model.Game;
import com.github.plugatarev.client.view.javafx.GameView;
import com.github.plugatarev.config.ConfigProperty;
import com.github.plugatarev.multicastreceiver.MulticastReceiver;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.SplitPane;
import javafx.stage.Stage;
import lombok.Setter;
import lombok.SneakyThrows;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;

public final class JavaFXStarter extends Application {
    private static final Logger logger = Logger.getLogger(JavaFXStarter.class);

    private static final String GAME_VIEW_FXML_PATH = "gameView.fxml";
    private static final String MULTICAST_ADDRESS = "239.192.0.4";
    private static final int MULTICAST_PORT = 9192;

    @Setter private static String playerName;
    @Setter private static NetworkInterface networkInterface;

    private MulticastReceiver multicastReceiver;
    private Game gameNetwork;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) {
        SnakesProto.GameConfig config = ConfigProperty.getConfig();

        try {
            InetSocketAddress multicastInfo = new InetSocketAddress(InetAddress.getByName(MULTICAST_ADDRESS), MULTICAST_PORT);

            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(JavaFXStarter.class.getClassLoader().getResource(GAME_VIEW_FXML_PATH));
            SplitPane root = loader.load();

            GameView view = loader.getController();
            gameNetwork = new Game(config, playerName, view, multicastInfo, networkInterface);
            JavaFXController gameController = new JavaFXController(config, playerName, gameNetwork, view);

            multicastReceiver = new MulticastReceiver(multicastInfo, view, gameNetwork, networkInterface);
            multicastReceiver.start();

            view.setStage(stage);
            view.setGameController(gameController);

            stage.setTitle(playerName);
            stage.setScene(new Scene(root));
            stage.sizeToScene();
            stage.show();
        }
        catch (IOException exception) {
            logger.error(exception.getLocalizedMessage());
        }
    }

    @Override
    public void stop() {
        if (multicastReceiver != null) multicastReceiver.stop();
        if (gameNetwork != null) gameNetwork.exit();
        System.exit(0);
    }
}
