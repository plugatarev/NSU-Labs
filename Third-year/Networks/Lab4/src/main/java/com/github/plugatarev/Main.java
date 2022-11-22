package com.github.plugatarev;

import org.apache.log4j.Logger;

import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.List;
import java.util.Scanner;

public final class Main {
    private static final Logger logger = Logger.getLogger(Main.class);
    private static final int COMMAND_LINE_ARGUMENTS_COUNT = 1;

    public static void main(String[] args) throws SocketException {
        if (args.length != COMMAND_LINE_ARGUMENTS_COUNT) {
            logger.error("Usage: player_name");
            return;
        }

        NetworkInterface networkInterface = getNetworkInterface();
        if (networkInterface == null) {
            logger.error("Not found network interface");
            return;
        }

        JavaFXStarter.setNetworkInterface(networkInterface);
        JavaFXStarter.setPlayerName(args[0]);
        JavaFXStarter.main(new String[0]);
    }

    private static NetworkInterface getNetworkInterface() throws SocketException {
        List<NetworkInterface> networkInterfaces = NetworkInterface.networkInterfaces().filter(netInterface -> {
            try {
                return netInterface.isUp() && !netInterface.isLoopback();
            } catch (SocketException ignored) {}
            return false;
        }).toList();

        if (networkInterfaces.isEmpty()) {
            logger.info("You don't have any working network interfaces.");
            return null;
        }
        return networkInterfaces.get(0);
    }
}
