import java.io.IOException;
import java.net.*;
import java.util.*;

public record CopiesDetector(InetAddress multicastAddress, NetworkInterface networkInterface) {
    private static final int PORT = 8888;
    private static final int TIMEOUT = 2000;
    private static final int DELAY = 0;
    private static final int BUFFER_SIZE = 0;

    private static final byte[] buffer = new byte[BUFFER_SIZE];

    private static final Timer sendTimer = new Timer(true);

    public static void detectCopies(InetAddress multicastAddress, NetworkInterface networkInterface) {
        CopiesDetector detector = new CopiesDetector(multicastAddress, networkInterface);
        detector.detectCopies();
    }

    private void detectCopies() {
        try (MulticastSocket receiveSocket = new MulticastSocket(PORT);
             MulticastSocket sendSocket = new MulticastSocket()) {

            receiveSocket.joinGroup(new InetSocketAddress(multicastAddress, PORT), networkInterface);
            receiveSocket.setSoTimeout(TIMEOUT);
            setSendTimer(sendSocket, multicastAddress);
            sendSocket.setNetworkInterface(networkInterface);

            HashMap<SocketAddress, Long> activeCopies = new HashMap<>();
            while (true) {
                boolean isConnectAddress = isConnectAddressTo(receiveSocket, activeCopies);
                boolean isDisconnectAddress = isDisconnectAddress(activeCopies);

                if (isConnectAddress || isDisconnectAddress) {
                    printActiveCopies(activeCopies);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private boolean isConnectAddressTo(MulticastSocket receiveSocket, HashMap<SocketAddress, Long> activeCopies) throws IOException {
        DatagramPacket receivePacket = new DatagramPacket(buffer, BUFFER_SIZE);
        try {
            receiveSocket.receive(receivePacket);
            SocketAddress receiveAddress = receivePacket.getSocketAddress();
            if (activeCopies.put(receiveAddress, System.currentTimeMillis()) == null) {
                System.out.println("New copy join with address: " + receiveAddress);
                return true;
            }
        } catch (SocketTimeoutException ignored) {}
        return false;
    }

    private boolean isDisconnectAddress(HashMap<SocketAddress, Long> activeCopies) {
        return activeCopies.entrySet().removeIf(e -> System.currentTimeMillis() - e.getValue() > 2 * TIMEOUT);
    }

    private void printActiveCopies(HashMap<SocketAddress, Long> activeCopies) {
        System.out.println(activeCopies.size() + " copies were detected:");
        for (SocketAddress address : activeCopies.keySet()) {
            System.out.println("Copy with address: " + address);
        }
    }

    private void setSendTimer(DatagramSocket socket, InetAddress multicastAddress) {
        sendTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                try {
                    socket.send(new DatagramPacket(buffer, BUFFER_SIZE, multicastAddress, PORT));
                } catch(IOException e) {
                    e.printStackTrace();
                }
            }
        }, DELAY, TIMEOUT);
    }
}
