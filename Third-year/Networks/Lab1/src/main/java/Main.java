import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;

public class Main {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Two argument required: <multicast address> <network interface>");
            return;
        }
        try {
            InetAddress multicastAddress = InetAddress.getByName(args[0]);
            if (!multicastAddress.isMulticastAddress()) {
                System.err.println("Specified address is not multicast");
                return;
            }
            NetworkInterface networkInterface = NetworkInterface.getByName(args[1]);
            if (!networkInterface.isUp()) {
                System.err.println("Specified interface is not active");
                return;
            }
            CopiesDetector.detectCopies(multicastAddress, networkInterface);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
