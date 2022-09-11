import java.io.IOException;
import java.net.*;
import java.util.*;

public record CopiesDetector(InetAddress multicastAddress) {
    private static final int PORT = 8888;
    private static final int TIMEOUT = 3000;
    private static final int DELAY = 0;
    private static final int BUFFER_SIZE = 0;

    private static final byte[] buffer = new byte[BUFFER_SIZE];

    private static final Timer sendTimer = new Timer(true);

    public static void detectCopies(InetAddress multicastAddress) {
        CopiesDetector detector = new CopiesDetector(multicastAddress);
        detector.detectCopies();
    }

    private void detectCopies() {
        try (MulticastSocket receiveSocket = new MulticastSocket(PORT);
             DatagramSocket sendSocket = new DatagramSocket()) {

            receiveSocket.joinGroup(multicastAddress);
            receiveSocket.setSoTimeout(TIMEOUT);

            Hashtable<SocketAddress, Long> activeCopies = new Hashtable<>();
            setTimer(sendSocket, multicastAddress);
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

    private boolean isConnectAddressTo(MulticastSocket receiveSocket, Hashtable<SocketAddress, Long> activeCopies) throws IOException {
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

    private boolean isDisconnectAddress(Hashtable<SocketAddress, Long> activeCopies) {
        for (Map.Entry<SocketAddress, Long> currentEntry : activeCopies.entrySet()) {
            if (System.currentTimeMillis() - currentEntry.getValue() > 3 * TIMEOUT) {
                SocketAddress curSocketAddress = currentEntry.getKey();
                System.out.println("Copy disconnected with address:" + curSocketAddress);
                activeCopies.remove(curSocketAddress);
                return true;
            }
        }
        return false;
    }

    private void printActiveCopies(Hashtable<SocketAddress, Long> activeCopies) {
        System.out.println(activeCopies.size() + " copies were detected:");
        for (SocketAddress address : activeCopies.keySet()) {
            System.out.println("Copy with address: " + address);
        }
    }

    private void setTimer(DatagramSocket socket, InetAddress multicastAddress) {
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
