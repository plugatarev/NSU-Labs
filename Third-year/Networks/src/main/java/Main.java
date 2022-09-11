import java.io.IOException;
import java.net.InetAddress;

public class Main {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("One argument required: multicast address");
            return;
        }
        try {
            InetAddress multicastAddress = InetAddress.getByName(args[0]);
            if (!multicastAddress.isMulticastAddress()){
                System.err.println("Specified address is not multicast");
                return;
            }
            CopiesDetector.detectCopies(multicastAddress);
        } catch (IOException e){
            e.printStackTrace();
        }
    }
}
